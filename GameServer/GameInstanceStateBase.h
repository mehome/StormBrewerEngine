#pragma once

#include "Foundation/Common.h"
#include "Foundation/Allocator/IdAllocator.h"

#include "Game/GameNetworkSettings.h"
#include "Game/GameMessages.refl.h"
#include "Game/GameNetworkData.refl.h"
#include "Game/GameSimulationEventCallbacks.h"
#include "Game/GameFullState.refl.h"
#include "Game/GameController.refl.h"
#include "Game/GameLogicContainer.h"
#include "Game/GameServerEventSender.h"
#include "Game/GameSharedGlobalResources.h"
#include "Game/GameSharedInstanceResources.h"
#include "Game/GameEventReconciler.h"

class GameClientConnection;
class GameServer;
class GameInstanceStateData;
struct GameInstanceStateDataPlayer;

class GameInstanceStateBase
{
public:

  GameInstanceStateBase(GameInstanceStateData & state_data) :
    m_StateData(state_data)
  {

  }

  virtual ~GameInstanceStateBase() {};

  virtual bool JoinPlayer(std::size_t client_index, const JoinGameMessage & join_game) { return false; };
  virtual void RemovePlayer(std::size_t client_index) {};

  virtual void Update() {};

  virtual void HandlePlayerReady(std::size_t client_index, const ReadyMessage & msg) {};
  virtual void HandlePlayerLoaded(std::size_t client_index, const FinishLoadingMessage & msg) {};

#if NET_MODE == NET_MODE_GGPO

  virtual void UpdatePlayer(std::size_t client_index, GameGGPOClientUpdate & update_data) {};

#else

  virtual void UpdatePlayer(std::size_t client_index, ClientAuthData & update_data) {};
  virtual void HandleClientEvent(std::size_t client_index, std::size_t class_id, void * event_ptr) {};
#endif

protected:

  GameInstanceStateData & m_StateData;
};


