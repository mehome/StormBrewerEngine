

#include "GameServer/GameInstance.h"
#include "GameServer/GameClientConnection.h"
#include "GameServer/GameInstanceStateBase.h"
#include "GameServer/GameInstanceStateStaging.h"
#include "GameServer.h"

#include "Game/GameMessages.refl.meta.h"
#include "Game/GameFullState.refl.meta.h"
#include "Game/GameStage.h"

#include <sb/vector.h>

#include <ctime>


GameInstance::GameInstance(GameServer & server, uint64_t game_id, const GameInitSettings & settings, 
                           GameSharedGlobalResources & global_resources, GameStageManager & stage_manager) :
  m_Server(server),
  m_GameId(game_id),
  m_StateData(this, settings, global_resources, stage_manager)
{
  Reset();
}

GameInstance::~GameInstance()
{

}

void GameInstance::Update()
{
  m_State->Update();

  if (m_NewState)
  {
    m_State = std::move(m_NewState);
  }
}

void GameInstance::Reset()
{
  m_State = std::make_unique<GameInstanceStateStaging>(m_StateData);
}

bool GameInstance::JoinPlayer(GameClientConnection * client, const JoinGameMessage & join_game)
{
  auto player_index = m_StateData.m_IdAllocator.Allocate();
  m_StateData.m_Players.EmplaceAt(player_index, GameInstanceStatePlayer{ client, join_game.m_UserName, (uint32_t)rand() });

  if (m_State->JoinPlayer(player_index, join_game) == false)
  {
    m_StateData.m_Players.RemoveAt(player_index);
    m_StateData.m_IdAllocator.Release(player_index);
    return false;
  }

  return true;
}

void GameInstance::RemovePlayer(GameClientConnection * client)
{
  auto player_index = GetPlayerIndex(client);

  m_StateData.m_Players.RemoveAt(player_index);
  m_StateData.m_IdAllocator.Release(player_index);

  m_State->RemovePlayer(player_index);
}

void GameInstance::HandlePlayerReady(GameClientConnection * client, const ReadyMessage & ready)
{
  auto player_index = GetPlayerIndex(client);
  m_State->HandlePlayerReady(player_index, ready);
}

void GameInstance::HandlePlayerLoaded(GameClientConnection * client, const FinishLoadingMessage & finish_loading)
{
  auto player_index = GetPlayerIndex(client);
  m_State->HandlePlayerLoaded(player_index, finish_loading);
}

#if NET_MODE == NET_MODE_GGPO

void GameInstance::UpdatePlayer(GameClientConnection * client, GameGGPOClientUpdate & update_data)
{
  auto player_index = GetPlayerIndex(client);
  m_State->UpdatePlayer(player_index, update_data);
}

#else

void GameInstance::UpdatePlayer(GameClientConnection * client, const ClientAuthData & auth_data)
{
  auto player_index = GetPlayerIndex(client);
  m_State->UpdatePlayer(player_index, auth_data);
}

void GameInstance::HandleClientEvent(GameClientConnection * client, std::size_t class_id, void * event_ptr)
{
  auto player_index = GetPlayerIndex(client);
  m_State->HandleClientEvent(player_index, class_id, event_ptr);
}

#endif

std::size_t GameInstance::GetNumPlayers()
{
  return m_StateData.m_Players.Size();
}

std::vector<GameClientConnection *> GameInstance::GetConnectedPlayers() const
{
  std::vector<GameClientConnection *> list;
  for (auto elem : m_StateData.m_Players)
  {
    list.push_back(elem.second.m_Client);
  }

  return list;
}

std::size_t GameInstance::GetPlayerIndex(NotNullPtr<GameClientConnection> client)
{
  for (auto elem : m_StateData.m_Players)
  {
    if (elem.second.m_Client == client)
    {
      return elem.first;
    }
  }

  throw false;
}

void GameInstance::SetNewState(std::unique_ptr<GameInstanceStateBase> && state)
{
  m_NewState = std::move(state);
}
