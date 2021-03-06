
#include "UserConnection.refl.meta.h"
#include "User.refl.meta.h"
#include "GameServerConnection.refl.meta.h"
#include "UserConnectionMessages.refl.meta.h"
#include "GameList.refl.meta.h"
#include "BanList.refl.meta.h"
#include "WelcomeInfo.refl.meta.h"
#include "GameServerConnection.refl.h"

#include "LobbyShared/LobbyValidation.h"

#include <hash/Hash64.h>

#include "HurricaneDDS/DDSRandom.h"

#include "StormRefl/StormReflJsonStd.h"

UserConnection::UserConnection(DDSNodeInterface node_interface) :
  m_Interface(node_interface), m_State(UserConnectionState::kLoadingUser), m_Error(false), m_Relocating(false)
{

}

void UserConnection::ConnectToEndpoint(DDSConnectionId connection_id)
{
  if (m_ConnectionId)
  {
    m_Interface.DisconnectLocalConnection(connection_id);
    return;
  }

  m_ConnectionId = connection_id;

  for (auto & data : m_PendingMessages)
  {
    m_Interface.SendDataToLocalConnection(*m_ConnectionId, data);
  }

  m_PendingMessages.clear();
}

void UserConnection::FinalizeUserLoaded()
{

  SendData(StormReflEncodeJson(UserMessageBase{"logged_in"}));

  m_State = UserConnectionState::kLoaded;

  m_Interface.Call(&User::AddEndpoint, m_UserId, m_Interface.GetLocalKey(), m_RemoteIP, m_RemoteHost);
  m_Interface.Call(&User::SetLocation, m_UserId, m_CountryCode, m_CurrencyCode);

#ifdef ENABLE_GAME_LIST
  m_Interface.CreateSubscription(DDSSubscriptionTarget<GameList>{}, 0, ".m_GameList", &UserConnection::HandleGameListUpdate, true);
#endif

#ifdef ENABLE_WELCOME_INFO
  m_Interface.CreateSubscription(DDSSubscriptionTarget<WelcomeInfo>{}, 0, ".m_Tabs", &UserConnection::HandleWelcomeInfoUpdate, true);
#endif
}

void UserConnection::PreDestroy()
{
  if (m_Relocating == false && m_State == UserConnectionState::kLoaded)
  {
    m_Interface.Call(&User::RemoveEndpoint, m_UserId, m_Interface.GetLocalKey());
  }
}

void UserConnection::PreMoveObject()
{
  DDSLog::LogVerbose("Moving connection to new server %llu", m_Interface.GetLocalKey());

  auto node_info = m_Interface.GetNodeInfo(m_Interface.GetLocalKey());
  if (node_info)
  {
    std::string ip_addr = GetNodeAddrAsString(node_info->get().m_Addr);
    UserMessageRelocate relocate{ "relocate", ip_addr, node_info->get().m_EndpointPorts[0], m_Interface.GetLocalKey() };
    SendData(StormReflEncodeJson(relocate));
    m_Relocating = true;
  }
  else
  {
    SendConnectionError("Server is going down for maintenance");
  }

  m_ConnectionId = {};
}


void UserConnection::LoadUser(std::string platform, uint64_t platform_id, uint64_t user_id,
    std::string remote_ip, std::string remote_host, std::string country_code, std::string currency_code, bool guest, std::string chosen_user_name)
{
#ifdef ENABLE_BAN_LIST
  auto ban_list = m_Interface.GetSharedObject<BanList>();
  if (ban_list->CheckBanList(remote_ip.c_str(), remote_host.c_str(), platform, platform_id))
  {
    SendConnectionError("You are banned");
    return;
  }

  m_Interface.CreateSubscription(DDSSubscriptionTarget<BanList>(), 0, "", &UserConnection::HandleBanListUpdate, false);
#endif
  m_RemoteIP = remote_ip;
  m_RemoteHost = remote_host;
  m_CountryCode = country_code;
  m_CurrencyCode = currency_code;
  m_Platform = platform;
  m_PlatformId = platform_id;
  m_UserId = user_id;
  m_IsGuest = guest;
  m_UserName = chosen_user_name;

  DDSLog::LogInfo("Loading User %zu", m_UserId);

  m_Interface.CreateSubscription(DDSSubscriptionTarget<User>{}, m_UserId, ".m_LocalInfo",
          &UserConnection::HandleLocalDataUpdate, true, &UserConnection::UserDoesntExist, true);
}

void UserConnection::CreateUserObject(const std::string & name)
{
  if (ValidUserName(name.c_str()) == false)
  {
    UserMessageBase msg{ "repick_new_user" };
    SendData(StormReflEncodeJson(msg));
    return;
  }

  UserDatabaseObject initial_data;
  initial_data.m_UserName = name;
  initial_data.m_UserNameLower = name;
  initial_data.m_Platform = m_Platform;
  initial_data.m_PlatformId = m_PlatformId;
  initial_data.m_Created = (int)m_Interface.GetNetworkTime();

  initial_data.m_IsGuest = m_IsGuest;
  initial_data.m_Icon = 0;
  initial_data.m_IconList.EmplaceBack(0);

  initial_data.m_Title = 0;
  initial_data.m_TitleList.EmplaceBack(0);

  initial_data.m_Celebration = 0;
  initial_data.m_CelebrationList.EmplaceBack(0);

  std::transform(initial_data.m_UserNameLower.begin(), initial_data.m_UserNameLower.end(), initial_data.m_UserNameLower.begin(), tolower);

  m_Interface.InsertIntoDatabase(initial_data, m_UserId, &UserConnection::HandleUserInsert, this);
  m_State = UserConnectionState::kCreatingNewUser;
}


void UserConnection::GotMessage(std::string cmd, std::string data)
{
  switch (m_State)
  {
  case UserConnectionState::kRequestingUserName:
    {
      UserMessageCreateName msg;
      if (StormReflParseJson(msg, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      CreateUserObject(msg.uname);
    }
    break;
  case UserConnectionState::kLoadingUser:
  case UserConnectionState::kCreatingNewUser:
    break;

  case UserConnectionState::kLoaded:
#ifdef ENABLE_CHANNELS
    if (cmd == "chat")
    {
      UserChatMessageIncoming chat_msg;
      if (StormReflParseJson(chat_msg, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      if (chat_msg.msg.length() == 0)
      {
        return;
      }

      if (chat_msg.msg[0] == '/')
      {
        m_Interface.Call(&User::ProcessSlashCommand, m_UserId, m_Interface.GetLocalKey(), chat_msg.channel_id, chat_msg.msg);
        return;
      }

      m_Interface.Call(&User::SendChatToChannel, m_UserId, m_Interface.GetLocalKey(), chat_msg.channel_id, chat_msg.msg);
    }
    else
#endif
    if (cmd == "set_title")
    {
      UserSetProfileVal req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SetTitle, m_UserId, req.val);
    }
    else if (cmd == "set_icon")
    {
      UserSetProfileVal req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SetIcon, m_UserId, req.val);
    }
    else if (cmd == "set_celeb")
    {
      UserSetProfileVal req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SetCelebration, m_UserId, req.val);
    }
    else
#ifdef ENABLE_SQUADS
    if (cmd == "set_primary_squad")
    {
      UserSetProfileSquad req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SetPrimarySquad, m_UserId, m_Interface.GetLocalKey(), req.val);
    }
    else
#endif
#ifdef ENABLE_WELCOME_INFO
    if (cmd == "edit_lobby_info")
    {
      UserEditInfo req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::UpdateWelcomeInfo, m_UserId, m_Interface.GetLocalKey(), req.data);
    }
    else
#endif
#if defined(ENABLE_CHANNELS) && defined(ENABLE_SQUADS)
    if (cmd == "edit_channel_info")
    {
      UserEditChannelInfo req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::UpdateChannelText, m_UserId, m_Interface.GetLocalKey(), req.channel_id, req.data);
    }
    else
#endif
    if (cmd == "create_game")
    {
      UserGameCreate req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::CreatePrivateGame, m_UserId, m_Interface.GetLocalKey(), req.create_data, req.password, req.zone_info);
    }
    else if (cmd == "join_game")
    {
      UserJoinGame req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      UserGameJoinInfo join_info;
      join_info.m_EndpointId = m_Interface.GetLocalKey();
      join_info.m_Password = req.password;
      join_info.m_Observer = req.observer;
      join_info.m_IntendedType = LobbyGameType::kPrivate;
      join_info.m_ZoneInfo = req.zone_info;

      m_Interface.Call(&User::JoinGameByLookupTable, m_UserId, req.join_code, join_info);
    }
    else if(cmd == "matchmake_casual")
    {
      UserMatchmakeRequest req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::StartMatchmakingCasual, m_UserId, req.playlist_mask, m_Interface.GetLocalKey(), req.zone_info);
    }
    else if(cmd == "matchmake_competitive")
    {
      UserMatchmakeRequest req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::StartMatchmakingCompetitive, m_UserId, req.playlist_mask, m_Interface.GetLocalKey(), req.zone_info);
    }
    else if(cmd == "cancel_matchmaking")
    {
      m_Interface.Call(&User::RemoveFromMatchmaking, m_UserId, m_Interface.GetLocalKey());
    }
    else if (cmd == "destroy_game")
    {
      UserDestroyGame req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::DestroyGame, m_UserId, m_Interface.GetLocalKey(), req.game_id);
    }
    else if (cmd == "game_chat")
    {
      UserChatMessageIncoming req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SendGameChat, m_UserId, m_Interface.GetLocalKey(), req.msg);
    }
    else if (cmd == "start_game")
    {
      m_Interface.Call(&User::StartGame, m_UserId);
    }
    else if (cmd == "change_ready")
    {
      UserReadyGame req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::ChangeReady, m_UserId, req.ready);
    }
#ifdef NET_USE_LOADOUT
    else if (cmd == "change_loadout")
    {
      UserSwitchLoadout req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::ChangeLoadout, m_UserId, req.loadout);
    }
#endif
    else if (cmd == "change_settings")
    {
      UserSwitchSettings req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::ChangeGameSettings, m_UserId, req.settings);
    }
    else if (cmd == "change_team")
    {
      UserSwitchTeam req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::SwitchTeams, m_UserId, req.target_user, req.team, m_Interface.GetLocalKey());
    }
    else if (cmd == "kick_game_user")
    {
      UserKickUserFromGame req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::KickUserFromGame, m_UserId, req.user);
    }
    else if (cmd == "leave_game")
    {
      m_Interface.Call(&User::RequestLeaveGame, m_UserId);
    }
    else if (cmd == "fetch_stats")
    {
      m_Interface.CallWithResponder(&User::FetchStats, m_UserId, &UserConnection::SendData, this);
    }
    else if (cmd == "fetch_user_stats")
    {
      UserFetchStats req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.CallWithResponder(&User::FetchStats, req.user, &UserConnection::SendData, this);
    }
    else
#ifdef ENABLE_GAME_LIST
    if (cmd == "preview_game")
    {
      UserPreviewGame req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      if (m_GamePreviewSubscription != 0)
      {
        m_Interface.DestroySubscription<GameServerConnection>(m_GamePreviewServerId, m_GamePreviewSubscription);
      }

      std::string game_path = ".m_GameList[" + std::to_string(req.game_id) + "]";
      m_GamePreviewSubscription = m_Interface.CreateSubscription(DDSSubscriptionTarget<GameServerConnection>{}, req.server_id, game_path.c_str(),
          &UserConnection::HandleGamePreviewUpdate, true, req.request_id, &UserConnection::HandleGamePreviewDestroyed);
      m_GamePreviewServerId = req.server_id;
      m_GamePreviewRequestId = req.request_id;
    }
    else if (cmd == "cancel_preview")
    {
      if (m_GamePreviewSubscription != 0)
      {
        m_Interface.DestroySubscription<GameServerConnection>(m_GamePreviewServerId, m_GamePreviewSubscription);
        m_GamePreviewSubscription = 0;
      }
    }
    else
#endif
#ifdef ENABLE_SQUADS
    if (cmd == "create_squad")
    {
      UserCreateSquad req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::CreateSquad, m_UserId, m_Interface.GetLocalKey(), req.name, req.tag);
    }
    else if (cmd == "disband_squad")
    {
      m_Interface.Call(&User::DestroySquad, m_UserId, m_Interface.GetLocalKey());
    }
    else if (cmd == "squad_apply")
    {
      UserSquadLookupAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::ApplyToSquad, m_UserId, m_Interface.GetLocalKey(), req.squad);
    }
    else if (cmd == "squad_apply_accept")
    {
      UserSquadMemberAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::AcceptSquadApplication, m_UserId, m_Interface.GetLocalKey(), req.squad, req.user);
    }
    else if (cmd == "squad_apply_decline")
    {
      UserSquadMemberAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::DeclineSquadApplication, m_UserId, m_Interface.GetLocalKey(), req.squad, req.user);
    }
    else if (cmd == "squad_apply_rescind")
    {
      UserSquadAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::RescindSquadApplication, m_UserId, m_Interface.GetLocalKey(), req.squad);
    }
    else if (cmd == "squad_invite")
    {
      UserSquadMemberLookupAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::RequestUserToJoinSquad, m_UserId, m_Interface.GetLocalKey(), req.squad, req.user_platform_id);
    }
    else if (cmd == "squad_invite_accept")
    {
      UserSquadAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::AcceptSquadRequest, m_UserId, m_Interface.GetLocalKey(), req.squad);
    }
    else if (cmd == "squad_invite_decline")
    {
      UserSquadAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::DeclineSquadRequest, m_UserId, m_Interface.GetLocalKey(), req.squad);
    }
    else if (cmd == "squad_invite_rescind")
    {
      UserSquadMemberAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::RescindSquadRequest, m_UserId, m_Interface.GetLocalKey(), req.squad, req.user);
    }
    else if (cmd == "squad_leave")
    {
      UserSquadAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::LeaveSquad, m_UserId, m_Interface.GetLocalKey(), req.squad);
    }
    else if (cmd == "squad_remove")
    {
      UserSquadMemberAction req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::RemoveSquadMember, m_UserId, m_Interface.GetLocalKey(), req.squad, req.user);
    }
    else if (cmd == "squad_permissions")
    {
      UserSquadMemberPermissions req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::EditSquadPermissions, m_UserId, m_Interface.GetLocalKey(), req.squad, req.member, req.rank);
    }
#ifdef ENABLE_CHANNELS
    else if (cmd == "squad_motd")
    {
      UserSquadMotd req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::EditSquadMotd, m_UserId, m_Interface.GetLocalKey(), req.squad, req.motd);
    }
    else
#endif
    if (cmd == "squad_lock")
    {
      UserSquadLock req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }
      m_Interface.Call(&User::LockSquadChannel, m_UserId, m_Interface.GetLocalKey(), req.squad, req.lock);
    }
    else
#endif
#ifdef ENABLE_CHANNELS
    if (cmd == "add_auto_join")
    {
      static_assert(std::is_convertible<std::tuple<std::decay_t<std::string &>>, std::tuple<std::decay_t<const std::string &>>>::value, "asdf");

      UserAutoJoinModify req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.CallWithResponder(&User::AddAutoJoinChannel, m_UserId, &UserConnection::SendNotification, this, req.channel);
    }
    else if (cmd == "rem_auto_join")
    {
      UserAutoJoinModify req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.CallWithResponder(&User::RemoveAutoJoinChannel, m_UserId, &UserConnection::SendNotification, this, req.channel);
    }
    else
#endif
#ifdef ENABLE_REWARDS
    if (cmd == "next_xp")
    {
      m_Interface.Call(&User::SendXPGain, m_UserId, m_Interface.GetLocalKey());
    }
    else if (cmd == "skip_xp")
    {
      m_Interface.Call(&User::SkipXPGain, m_UserId);
    }
    else
#endif
    if (cmd == "change_persistent")
    {
      UserPersistentModify req;
      if (StormReflParseJson(req, data.c_str()) == false)
      {
        SendConnectionError("Parse error");
        return;
      }

      m_Interface.Call(&User::ModifyPersistent, m_UserId, req.change);
    }

    break;
  }
}

void UserConnection::UserDoesntExist()
{
  if(m_UserName.empty())
  {
    DDSLog::LogVerbose("Requesting New User Name");
    m_State = UserConnectionState::kRequestingUserName;

    UserMessageBase msg{ "new_user" };
    SendData(StormReflEncodeJson(msg));
  }
  else
  {
    CreateUserObject(m_UserName);
  }
}

void UserConnection::SendData(std::string data)
{
  DDSLog::LogInfo("Sending %s", data.c_str());
  if (m_ConnectionId)
  {
    m_Interface.SendDataToLocalConnection(*m_ConnectionId, data);
  }
  else
  {
    m_PendingMessages.emplace_back(data);
  }
}

void UserConnection::SendNotification(std::string msg)
{
  UserMessageRuntimeError err{ "smsg", msg };
  SendData(StormReflEncodeJson(err));
}

void UserConnection::SendServerText(std::string msg)
{
  UserMessageRuntimeError err{ "stxt", msg };
  SendData(StormReflEncodeJson(err));
}

void UserConnection::SendRuntimeError(std::string msg)
{
  UserMessageRuntimeError err{ "rterr", msg };
  SendData(StormReflEncodeJson(err));
}

void UserConnection::SendConnectionError(std::string msg)
{
  UserMessageConnectionError err{ "error", msg };
  SendData(StormReflEncodeJson(err));

  if (m_ConnectionId)
  {
    m_Interface.DisconnectLocalConnection(*m_ConnectionId);
  }
  else
  {
    m_Error = true;
  }
}

void UserConnection::HandleUserInsert(int ec)
{
  if (ec == 0)
  {
    FinalizeUserLoaded();
  }
  else
  {
    m_State = UserConnectionState::kRequestingUserName;

    UserMessageBase msg{ "repick_new_user" };
    SendData(StormReflEncodeJson(msg));
  }
}

void UserConnection::HandleLocalDataUpdate(std::string data)
{
  if (m_State != UserConnectionState::kLoaded)
  {
    FinalizeUserLoaded();
  }

  m_Interface.SendDataToLocalConnection(*m_ConnectionId, StormReflEncodeJson(UserLocalInfoUpdate{ "local", data }));
}

#ifdef ENABLE_GAME_LIST
void UserConnection::HandleGameListUpdate(std::string data)
{
  m_Interface.SendDataToLocalConnection(*m_ConnectionId, StormReflEncodeJson(UserLocalInfoUpdate{ "server", data }));
}
#endif

#ifdef ENABLE_WELCOME_INFO
void UserConnection::HandleWelcomeInfoUpdate(std::string data)
{
  m_Interface.SendDataToLocalConnection(*m_ConnectionId, StormReflEncodeJson(UserLocalInfoUpdate{ "winfo", std::move(data) }));
}
#endif

#ifdef ENABLE_BAN_LIST
void UserConnection::HandleBanListUpdate(std::string data)
{
  auto ban_list = m_Interface.GetSharedObject<BanList>();
  if (ban_list->CheckBanList(m_RemoteIP.c_str(), m_RemoteHost.c_str(), m_Platform, m_PlatformId))
  {
    SendConnectionError("You are banned");
  }
}
#endif

#ifdef ENABLE_GAME_LIST
void UserConnection::HandleGamePreviewUpdate(int request_id, std::string data)
{
  UserGamePreviewUpdate update;
  update.c = "game_preview";
  update.data = data;
  update.request_id = request_id;

  m_Interface.SendDataToLocalConnection(*m_ConnectionId, StormReflEncodeJson(update));
}

void UserConnection::HandleGamePreviewDestroyed(int request_id)
{
  UserGamePreviewCancel update;
  update.c = "game_preview_cancel";
  update.request_id = request_id;

  m_Interface.SendDataToLocalConnection(*m_ConnectionId, StormReflEncodeJson(update));
}

#endif
