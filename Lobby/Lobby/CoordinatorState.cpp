
#include "CoordinatorState.h"
#include "DataTypes.h"

#include "ProjectSettings/ProjectPorts.h"
#include "ProjectSettings/ProjectName.h"

#include <HurricaneDDS/DDSDatabaseConnection.h>
#include <HurricaneDDS/DDSDatabaseBootstrap.h>

std::unique_ptr<DDSCoordinatorState> CreateCoordinatorState(bool reset_db, const char * database_host_addr, int database_port)
{
  StormSockets::StormSocketInitSettings backend_settings;
  backend_settings.NumIOThreads = 2;
  backend_settings.NumSendThreads = 2;

  DDSDatabaseSettings database_settings;
  database_settings.DatabaseHostName = database_host_addr;
  database_settings.DatabasePort = database_port;
  database_settings.DatabaseName = DATABASE_NAME;
  database_settings.NumThreads = 1;

  StormSockets::StormSocketClientFrontendHttpSettings http_client_settings;

  if (reset_db)
  {
    std::vector<DDSDatabaseBoostrapCollectionInfo> additional_collections;

    DDSDatabaseBoostrapCollectionInfo gifts_collection;
    gifts_collection.m_CollectionName = "Gifts";

    additional_collections.emplace_back(gifts_collection);

    DDSDatabaseBoostrapFull(DataObjectList{}, database_settings);
    DDSDatabaseConnection connection(database_settings);

#ifdef ENABLE_CHANNELS
    BuiltInChannelDatabaseObject channel_data;
    channel_data.m_ChannelNameLower = "general";
    channel_data.m_ChannelInfo.m_ChannelName = "General";
    channel_data.m_ChannelInfo.m_Motd = "Welcome!";
    channel_data.m_ChannelInfo.m_RequiredAdminLevel = 0;
    connection.QueryDatabaseInsert(Channel::GetChannelKeyFromName("General"), "BuiltInChannel", StormReflEncodeJson(channel_data));

    channel_data.m_ChannelNameLower = "admin";
    channel_data.m_ChannelInfo.m_ChannelName = "Admin";
    channel_data.m_ChannelInfo.m_Motd = "Hey it's the admin channel";
    channel_data.m_ChannelInfo.m_RequiredAdminLevel = 1;
    connection.QueryDatabaseInsert(Channel::GetChannelKeyFromName("Admin"), "BuiltInChannel", StormReflEncodeJson(channel_data));

    channel_data.m_ChannelNameLower = "support";
    channel_data.m_ChannelInfo.m_ChannelName = "Support";
    channel_data.m_ChannelInfo.m_Motd = "Please ask your question and let one of our support staff get back to you";
    channel_data.m_ChannelInfo.m_RequiredAdminLevel = 0;
    connection.QueryDatabaseInsert(Channel::GetChannelKeyFromName("Support"), "BuiltInChannel", StormReflEncodeJson(channel_data));

    channel_data.m_ChannelNameLower = "europe";
    channel_data.m_ChannelInfo.m_ChannelName = "Europe";
    channel_data.m_ChannelInfo.m_Motd = ":gr::fi::ge::is::gb::no::dk::ch::mt::pl::lv::at::lu::nl::hr::rs::sk::si::ru::cz::fr::ro::md::be::de::am::az::lt::bg::hu::ie::it::pt::by::tr::al::me::mk::es::ua::ba::se::kz::ee::il::cy:";
    channel_data.m_ChannelInfo.m_RequiredAdminLevel = 0;
    connection.QueryDatabaseInsert(Channel::GetChannelKeyFromName("Europe"), "BuiltInChannel", StormReflEncodeJson(channel_data));

#endif

#ifdef ENABLE_AUTH_STEAM
    UserDatabaseObject default_admin;
    default_admin.m_AdminLevel = 9;
    default_admin.m_Title = 0;
    default_admin.m_UserName = "NickW";
    default_admin.m_UserNameLower = "nickw";
    default_admin.m_Platform = "steam";
    default_admin.m_PlatformId = 76561197970016586ULL;

    connection.QueryDatabaseInsert(User::GetUserIdForPlatformId("steam", 76561197970016586), "User", StormReflEncodeJson(default_admin));

    default_admin.m_UserName = "StormBrewers";
    default_admin.m_UserNameLower = "stormbrewers";
    default_admin.m_PlatformId = 76561198232963580ULL;
    connection.QueryDatabaseInsert(User::GetUserIdForPlatformId("steam", 76561198232963580), "User", StormReflEncodeJson(default_admin));
#endif
  }

  StormSockets::StormSocketServerFrontendWebsocketSettings coordinator_settings;
  coordinator_settings.ListenSettings.Port = COORDINATOR_PORT;
  StormSockets::StormSocketServerFrontendWebsocketSettings lb_settings;
  lb_settings.ListenSettings.Port = COORDINATOR_LB_PORT;
  lb_settings.Protocol = kProjectName;

  auto coordinator = 
    std::make_unique<DDSCoordinatorState>(DataObjectList{}, SharedObjectList{}, backend_settings, coordinator_settings, http_client_settings, database_settings);

  coordinator->InitializeSharedObjects();
  coordinator->InitializeLoadBalancerServer(lb_settings, 0);
  return std::move(coordinator);
}

