#pragma once

#include "Foundation/Common.h"

#include "Engine/Entity/EntityHandle.h"

#include "Runtime/ServerObject/ServerObjectManager.h"

class GameContainer;
class GameClientInstanceContainer;

class GameClientEntitySync
{
public:
  GameClientEntitySync(GameContainer & game);
  ~GameClientEntitySync();

  void ActivateEntities();
  void Sync(GameClientInstanceContainer & instance_container);
  void DestroyAll();

  void SendEntityEvent(int entity_index, uint32_t type_name_hash, const void * ev);

private:
  GameContainer & m_GameContainer;
  SparseList<EntityHandle> m_Entities;

  bool m_ActivateEntities = false;
};

