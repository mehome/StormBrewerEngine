
#include <HurricaneDDS/DDSLog.h>
#include <HurricaneDDS/DDSRandom.h>
#include <StormRefl/StormReflJsonStd.h>
#include <StormData/StormDataJsonUtil.h>
#include <StormSockets/StormSha1.h>

#include "UserEndpoint.h"
#include "User.refl.h"

#include "LobbyConfig.h"

#include "UserConnection.refl.meta.h"
#include "UserConnectionMessages.refl.meta.h"
#include "SteamServerMessages.refl.meta.h"

#ifdef ENABLE_AUTH_STEAM
char g_SteamValidationUrl[] = "https://api.steampowered.com/ISteamUserAuth/AuthenticateUserTicket/v1/?key=" STEAM_WEB_API_KEY "&format=json&appid=" STEAM_APP_ID "&ticket=";
char g_SteamOwnershipUrl[] = "https://api.steampowered.com/ISteamUser/GetPublisherAppOwnership/v2/?key=" STEAM_WEB_API_KEY "&format=json&appid=" STEAM_APP_ID "&steamid=";
char g_SteamInfoUrl[] = "https://api.steampowered.com/ISteamMicroTxn/GetUserInfo/V0001/?key=" STEAM_WEB_API_KEY "&steamid=";
#endif

UserEndpoint::UserEndpoint(const DDSEndpointInterface & endpoint_interface) :
  m_EndpointInterface(endpoint_interface), m_State(kVersionRequest), m_Error(false), m_PlatformId(0), m_ConnectionKey(0)
{
  DDSLog::LogInfo("Got user connect from %s:%d", m_EndpointInterface.GetRemoteIpAsString().data(), m_EndpointInterface.GetRemotePort());

  UserVersionRequest version_request;
  version_request.c = "version";

#ifdef ENABLE_PROOF_OF_WORK
  uint64_t prefix = DDSGetRandomNumber64();
  m_ProofVal = DDSGetRandomNumberRange(0, 100000);

  char proof_str[128];
  snprintf(proof_str, sizeof(proof_str), "%llu%d", (unsigned long long)prefix, m_ProofVal);

  std::string sha;
  StormSockets::StormSha1::CalcHash(proof_str, sha);

  version_request.sha = sha;
  version_request.prefix = std::to_string(prefix);
#endif

  m_EndpointInterface.Send(version_request);
}

UserEndpoint::~UserEndpoint()
{
  DDSLog::LogInfo("Lost user connect from %s:%d", m_EndpointInterface.GetRemoteIpAsString().data(), m_EndpointInterface.GetRemotePort());
}

void UserEndpoint::HandleData(const char * data)
{
  if (m_Error)
  {
    return;
  }

  DDSLog::LogInfo("Got user data %s", data);

  const char * cmd_start = StormDataFindJsonStartByPath(".c", data);
  std::string cmd;

  if (cmd_start)
  {
    StormReflParseJson(cmd, cmd_start);
  }

  switch (m_State)
  {
  case kVersionRequest:
    {
      UserMessageVersionResponse msg;
      if (StormReflParseJson(msg, data) == false)
      {
        ConnectionError("Parse error");
        return;
      }

      if (msg.ver != LOBBY_VERSION)
      {
        ConnectionError("Your version is out of date.<br />Restart the client to update.");
        return;
      }

#ifdef ENABLE_PROOF_OF_WORK
      if (msg.sha != m_ProofVal)
      {
        ConnectionError("Invalid version response");
        return;
      }
#endif

      DDSLog::LogVerbose("Looking up reverse host name");
      m_State = kLookup;
      m_EndpointInterface.CreateResolverRequest(m_Callback, m_EndpointInterface.GetRemoteIpAsString().data(), true,
        [this](const DDSResolverRequest & resolver_data) { HandleReverseLookup(resolver_data); });
    }
    break;
  case kIdentify:
    {
      DDSLog::LogVerbose("Got Identify Response");

      UserMessageIdentifyResponse response;
      if (StormReflParseJson(response, data) == false)
      {
        ConnectionError("Parse error");
        return;
      }
#ifdef ENABLE_AUTH_GUEST
      else if (response.c == "lguest")
      {
        m_IsGuest = true;
        m_ChosenUserName = response.user_name;
        m_Platform = "guest";
        m_PlatformId = DDSGetRandomNumber64();
        m_CountryCode = "us";
        m_CurrencyCode = "usd";

        DDSLog::LogInfo("Logging in with guest account - %zu", m_PlatformId);
        SetGuestUserId(m_PlatformId);
        FinalizeConnect();
      }
#endif

#ifdef ENABLE_AUTH_STEAM
      else if (response.c == "lextsteam")
      {
        DDSLog::LogVerbose("Logging in with external steam token");

        if (m_EndpointInterface.NodeIsReady() == false)
        {
          ConnectionError("Server Error");
          return;
        }

        try
        {
          uint64_t token = std::stoull(response.token);

          m_State = kExternalTokenValidation;
          m_EndpointInterface.CreateTokenValidationRequest(m_Callback, token, 0,
            [this](bool valid, const std::string & steam_id) { HandleSteamTokenValidation(valid, steam_id); });
        }
        catch (...)
        {
          ConnectionError("Server Error");
          return;
        }
      }
      else if (response.c == "ltsteam")
      {
        DDSLog::LogVerbose("Logging in with steam token");

        if (response.token.find_first_of('&') != std::string::npos)
        {
          ConnectionError("Invalid token");
          return;
        }

        std::string validation_url = std::string(g_SteamValidationUrl) + response.token;

        m_EndpointInterface.CreateHttpRequest(m_Callback, DDSHttpRequest(validation_url),
          [this](bool success, std::string data, std::string headers) { HandleSteamAuthenticationRequest(success, data.data(), headers.data()); });

        m_State = kSteamTokenValidation;
      }
#endif
      else if (response.c == "lr")
      {
        DDSLog::LogVerbose("Logging in with relocate token");

        DDSKey connection_key;

        try
        {
          connection_key = std::stoull(response.token);
        }
        catch (...)
        {
          ConnectionError("Server Error");
          return;
        }

        if (m_EndpointInterface.IsLocalKey(connection_key))
        {
          m_ConnectionKey = connection_key;
          if (m_EndpointInterface.ConnectToLocalObject(&UserConnection::ConnectToEndpoint, m_ConnectionKey) == false)
          {
            ConnectionError("Invalid credentials");
            return;
          }

          UserMessageBase success_msg = { "relocated" };          
          m_EndpointInterface.Send(success_msg);

          m_State = kConnectedToObj;
        }
        else
        {
          auto node_info = m_EndpointInterface.GetNodeInfo(connection_key);
          if (node_info)
          {
            std::string ip_addr = GetNodeAddrAsString(node_info->get().m_Addr);
            UserMessageRelocate relocate{ "relocate", ip_addr, node_info->get().m_EndpointPorts[0], connection_key };
            m_EndpointInterface.Send(relocate);
          }
          else
          {
            m_EndpointInterface.ForceDisconnect();
            m_State = kDisconnecting;
          }
        }
      }
      else
      {
        ConnectionError("Invalid credentials");
        return;
      }
    }
    break;
  case kConnectedToObj:
    {
      m_EndpointInterface.Call(&UserConnection::GotMessage, m_ConnectionKey, cmd, std::string(data));
    }
    break;
  default:
    ConnectionError("Invalid response");
    return;
  }
}

void UserEndpoint::HandleDisconnect()
{
  if (m_State == kConnectedToObj)
  {
    m_EndpointInterface.DestroyObject<UserConnection>(m_ConnectionKey);
  }
}

void UserEndpoint::ConnectionError(const char * err_msg)
{
  UserMessageConnectionError err{ "error", err_msg };
  m_EndpointInterface.Send(err);

  m_Error = true;
}

void UserEndpoint::HandleReverseLookup(const DDSResolverRequest & resolver_data)
{
  if (m_Error)
  {
    DDSLog::LogVerbose("Got reverse hostname lookup error");
    return;
  }

  DDSLog::LogVerbose("Got reverse hostname lookup result");

  if (resolver_data.m_Success == false)
  {
    m_RemoteHostName = m_EndpointInterface.GetRemoteIpAsString();
  }
  else
  {
    m_RemoteHostName = resolver_data.m_Result;
  }

  DDSLog::LogVerbose("Looking up tor blacklist");
  m_State = kTorBlacklist;

  uint32_t remote_ip = m_EndpointInterface.GetRemoteIp();
  uint8_t * ip_addr = (uint8_t *)&remote_ip;

  char tor_lookup_name[128];
  snprintf(tor_lookup_name, sizeof(tor_lookup_name), "%d.%d.%d.%d.tor.dan.me.uk", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);

  m_EndpointInterface.CreateResolverRequest(m_Callback, tor_lookup_name, false,
    [this](const DDSResolverRequest & resolver_data) { HandleTorBlacklistLookup(resolver_data); });
}

void UserEndpoint::HandleTorBlacklistLookup(const DDSResolverRequest & resolver_data)
{
  if (resolver_data.m_Success)
  {
    ConnectionError("Invalid host");
    return;
  }

  DDSLog::LogVerbose("Sending identify");
  m_State = kIdentify;
  UserMessageIdentifyRequest ident_request = { "identify" };
  m_EndpointInterface.Send(ident_request);
}

#ifdef ENABLE_AUTH_GUEST
void UserEndpoint::SetGuestUserId(uint64_t guest_id)
{
  m_Platform = "guest";
  m_PlatformId = guest_id;
  m_UserId = User::GetUserIdForPlatformId(m_Platform, m_PlatformId);
}
#endif


#ifdef ENABLE_AUTH_STEAM
void UserEndpoint::HandleSteamTokenValidation(bool valid, const std::string & steam_id)
{
  if (steam_id.length() == 0)
  {
    ConnectionError("Couldn't validate steam token");
    return;
  }

  if (m_State == kExternalTokenValidation)
  {
    FinalizeSteamValidation(std::stoull(steam_id));
  }
}

void UserEndpoint::HandleSteamAuthenticationRequest(bool success, const char * data, const char * headers)
{
  if (success == false)
  {
    ConnectionError("Steam validation error");
    return;
  }

  SteamValidation validation;
  if (StormReflParseJson(validation, data) == false)
  {
    ConnectionError("Steam authentication error");
    return;
  }

  if (validation.response.params.result != "OK")
  {
    ConnectionError("Could not validate Steam Id");
    return;
  }

  if (validation.response.params.vacbanned || validation.response.params.publisherbanned)
  {
    ConnectionError("Your steam account has been banned");
    return;
  }

  uint64_t steam_id;
  if (StormReflParseJson(steam_id, validation.response.params.steamid.data()) == false)
  {
    ConnectionError("Parse error");
    return;
  }

  FinalizeSteamValidation(steam_id);
}

void UserEndpoint::HandleSteamOwnershipRequest(bool success, const char * data, const char * headers)
{
  SteamOwnership validation;
  if (StormReflParseJson(validation, data) == false)
  {
    ConnectionError("Parse error");
    return;
  }

  if (validation.appownership.apps.size() == 0)
  {
    ConnectionError("Could not validate Steam Id");
    return;
  }

  if (validation.appownership.apps[0].ownsapp == false)
  {
    ConnectionError("You do not own the game on Steam!");
    return;
  }

  m_State = kSteamUserInfo;

  std::string info_url = std::string(g_SteamInfoUrl) + std::to_string(m_PlatformId);

  DDSLog::LogVerbose("Checking steam user info");
  m_EndpointInterface.CreateHttpRequest(m_Callback, DDSHttpRequest(info_url),
    [this](bool success, std::string data, std::string headers) { HandleSteamUserInfoRequest(success, data.data(), headers.data()); });
}

void UserEndpoint::HandleSteamUserInfoRequest(bool success, const char * data, const char * headers)
{
  SteamTxnUserInfo user_info;
  if (StormReflParseJson(user_info, data) == false)
  {
    ConnectionError("Parse error");
    return;
  }

  if (user_info.response.result != "OK")
  {
    ConnectionError("Could not validate Steam Id");
    return;
  }

  m_CountryCode = user_info.response.params.country;
  m_CurrencyCode = user_info.response.params.currency;

  SetSteamUserId(m_PlatformId);
  FinalizeConnect();
}

void UserEndpoint::SetSteamUserId(uint64_t steam_id)
{
  m_Platform = "steam";
  m_PlatformId = steam_id;
  m_UserId = User::GetUserIdForPlatformId(m_Platform, m_PlatformId);
}

void UserEndpoint::FinalizeSteamValidation(uint64_t steam_id)
{
  m_State = kSteamOwnershipValidation;
  SetSteamUserId(steam_id);

  std::string ownership_url = std::string(g_SteamOwnershipUrl) + std::to_string(steam_id);

  DDSLog::LogVerbose("Checking steam appid ownership");
  m_EndpointInterface.CreateHttpRequest(m_Callback, DDSHttpRequest(ownership_url),
    [this](bool success, std::string data, std::string headers) { HandleSteamOwnershipRequest(success, data.data(), headers.data()); });
}
#endif

void UserEndpoint::FinalizeConnect()
{
  DDSLog::LogVerbose("Creating User Connection");
  m_State = kCreateConnectionObject;

  if (m_EndpointInterface.CreateObject<UserConnection>(m_ConnectionKey) == false)
  {
    m_EndpointInterface.CreateTimer(m_Callback, std::chrono::seconds(1), [this] { FinalizeConnect(); });
    return;
  }

  m_EndpointInterface.ConnectToLocalObject(&UserConnection::ConnectToEndpoint, m_ConnectionKey);
  m_EndpointInterface.Call(&UserConnection::LoadUser, m_ConnectionKey, m_Platform, m_PlatformId, m_UserId,
      m_EndpointInterface.GetRemoteIpAsString(), m_RemoteHostName, m_CountryCode, m_CurrencyCode, m_IsGuest, m_ChosenUserName);

  m_State = kConnectedToObj;
}
