

#include "Game/GameCommon.h"

#include "GameShared/Systems/GameLogicSystems.h"
#include "GameShared/GameLogicContainer.h"

#include "Game/GameServerEventSender.h"
#include "Game/GameStage.h"
#include "Game/GameCollision.refl.h"

#include "Game/ServerObjects/GameServerObjectBase.refl.h"
#include "Game/ServerObjects/GameServerObjectBase.refl.meta.h"
#include "GameServerObjectBase.refl.h"


CLIENT_ASSET(ClientAssetType::kEntity, "./Entities/DefaultServerObject.entity", g_DefaultServerObjectEntity);

SpritePtr g_DefaultEmptrySpritePtr;

void GameServerObjectBase::Init(const GameServerObjectBaseInitData & init_data, GameLogicContainer & game_container)
{
  
}

void GameServerObjectBase::UpdateFirst(GameLogicContainer & container)
{
  m_CollisionId.Clear();
  container.GetSystems().GetCollisionDatabase().GetCollisionId(m_CollisionId);
}

void GameServerObjectBase::InitPosition(const Vector2 & pos)
{
  m_Position = GameNetVec2(pos.x, pos.y);
}

GameNetVec2 GameServerObjectBase::GetPosition() const
{
  return m_Position;
}

void GameServerObjectBase::SetPosition(const GameNetVec2 & pos)
{
  m_Position = pos;
}

Optional<AnimationState> GameServerObjectBase::GetAnimationState() const
{
  return {};
}

void GameServerObjectBase::SetAnimationState(const AnimationState & anim_state)
{

}

void GameServerObjectBase::SetAssociatedPlayer(int associated_player) const
{

}

bool GameServerObjectBase::FrameAdvance(uint32_t anim_name_hash, bool loop, int frames)
{
  auto anim_state = GetAnimationState();
  auto & sprite = GetSprite();
  if (anim_state && sprite.IsLoaded())
  {
    bool result = sprite->FrameAdvance(anim_name_hash, anim_state.Value(), loop, frames);
    SetAnimationState(anim_state.Value());

    return result;
  }

  return false;
}

void GameServerObjectBase::ResetAnimState()
{
  SetAnimationState(AnimationState{});
}

void GameServerObjectBase::PushDealDamageEventBox(const Box & b, const DamageEvent & damage_event, GameLogicContainer & game_container)
{
  EventMetaData new_meta(this, &game_container);

  auto facing = GetFacing();

  Box box = b;
  if (facing && facing.Value() == CharacterFacing ::kLeft)
  {
    box = box.MirrorX();
  }

  box = box.Offset(m_Position);

  auto dmg = game_container.GetServerObjectEventSystem().PushEventSource(box, new_meta, EventDef<DamageEvent>{});
  *dmg = damage_event;
}

void GameServerObjectBase::PushDealDamageEventBoxes(uint32_t multi_box_name_hash, const DamageEvent & damage_event, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  auto animation_state = GetAnimationState();
  if(sprite && animation_state)
  {
    auto frame_id = sprite->GetAnimationFrameId(animation_state->m_AnimIndex, animation_state->m_AnimFrame);

    auto boxes = sprite->GetMultiBox(multi_box_name_hash, frame_id);
    for (auto &box : boxes)
    {
      PushReceiveDamageEventBox(box, game_container);
    }
  }
}

void GameServerObjectBase::PushDealDamageEventBox(uint32_t box_name_hash, const DamageEvent & damage_event, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  if(sprite)
  {
    auto box = sprite->GetSingleBoxDefault(box_name_hash).Offset(m_Position);
    PushDealDamageEventBox(box, damage_event, game_container);
  }
}

void GameServerObjectBase::PushReceiveDamageEventBox(const Box & b, GameLogicContainer & game_container)
{
  auto facing = GetFacing();

  Box box = b;
  if (facing && facing.Value() == CharacterFacing::kLeft)
  {
    box = box.MirrorX();
  }

  box = box.Offset(m_Position);
  game_container.GetServerObjectEventSystem().PushEventReceiver<DamageEvent>(GetObjectHandle(), box);
}

void GameServerObjectBase::PushReceiveDamageEventBox(uint32_t box_name_hash, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  if(sprite)
  {
    auto box = sprite->GetSingleBoxDefault(box_name_hash).Offset(m_Position);
    PushReceiveDamageEventBox(box, game_container);
  }
}

void GameServerObjectBase::PushReceiveDamageEventBoxes(uint32_t multi_box_name_hash, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  auto animation_state = GetAnimationState();
  if(sprite && animation_state)
  {
    auto frame_id = sprite->GetAnimationFrameId(animation_state->m_AnimIndex, animation_state->m_AnimFrame);

    auto boxes = sprite->GetMultiBox(multi_box_name_hash, frame_id);
    for (auto &box : boxes)
    {
      PushReceiveDamageEventBox(box, game_container);
    }
  }
}

void GameServerObjectBase::PushReceiveDamageCollisionBox(const Box & b, GameLogicContainer & game_container)
{
  game_container.GetSystems().GetCollisionDatabase().PushDynamicCollision(b,
          (uint32_t)GameCollisionType::kCollisionDamagable, CollisionDatabaseObjectInfo(GetObjectHandle()), m_CollisionId);
}

void GameServerObjectBase::PushReceiveDamageCollisionBox(uint32_t box_name_hash, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  if(sprite)
  {
    auto box = sprite->GetSingleBoxDefault(box_name_hash).Offset(m_Position);
    PushReceiveDamageCollisionBox(box, game_container);
  }
}

void GameServerObjectBase::PushReceiveDamageCollisionBoxes(uint32_t multi_box_name_hash, GameLogicContainer & game_container)
{
  auto & sprite = GetSprite();
  auto animation_state = GetAnimationState();
  if(sprite && animation_state)
  {
    auto frame_id = sprite->GetAnimationFrameId(animation_state->m_AnimIndex, animation_state->m_AnimFrame);

    auto boxes = sprite->GetMultiBox(multi_box_name_hash, frame_id);
    for (auto &box : boxes)
    {
      PushReceiveDamageCollisionBox(box, game_container);
    }
  }
}

void GameServerObjectBase::PushCVCBox(const Box & b, GameLogicContainer & game_container)
{
  game_container.GetSystems().GetCVCPushSystem().SetCharacterCVCPosition(b, this);
}

void GameServerObjectBase::PushCVCBox(uint32_t box_name_hash, GameLogicContainer &game_container)
{
  auto & sprite = GetSprite();
  if(sprite)
  {
    auto box = sprite->GetSingleBoxDefault(box_name_hash).Offset(m_Position);
    PushCVCBox(box, game_container);
  }
}

const SpritePtr & GameServerObjectBase::GetSprite() const
{
  return g_DefaultEmptrySpritePtr;
}

Optional<CharacterFacing> GameServerObjectBase::GetFacing() const
{
  return {};
}


Optional<int> GameServerObjectBase::GetCollisionId() const
{
  return m_CollisionId;
}

#ifdef MOVER_ONE_WAY_COLLISION
MoverResult GameServerObjectBase::MoveCheckCollisionDatabase(GameLogicContainer & game_container, const GameNetVec2 & velocity, bool fallthrough)
#else
MoverResult GameServerObjectBase::MoveCheckCollisionDatabase(GameLogicContainer & game_container, const GameNetVec2 & velocity)
#endif
{
  auto & stage = game_container.GetStage();
  auto & collision = game_container.GetSystems().GetCollisionDatabase();

  auto & sprite = GetSprite();
  auto move_box = sprite->GetSingleBoxDefault(COMPILE_TIME_CRC32_STR("MoveBox"));

  MoveRequest req = Mover::CreateMoveRequest(m_Position, velocity, move_box);

#ifdef MOVER_ONE_WAY_COLLISION
  auto result = Mover::UpdateMover(collision, req, (int)GameCollisionType::kCollisionSolid,
          fallthrough ? 0 : (int)GameCollisionType::kCollisionOneWay);
#else
  auto result = Mover::UpdateMover(collision, req, 0xFFFFFFFF);
#endif

  if (result.m_HitRight)
  {
    m_Position.x = IntersectionFuncs<GameNetVal>::NextLowest(result.m_ResultPos.x + 1);
  }
  else if (result.m_HitLeft)
  {
    m_Position.x = result.m_ResultPos.x;
  }
  else
  {
    m_Position.x += velocity.x;
  }

  if (result.m_HitTop)
  {
    m_Position.y = IntersectionFuncs<GameNetVal>::NextLowest(result.m_ResultPos.y + 1);
  }
  else if (result.m_HitBottom)
  {
    m_Position.y = result.m_ResultPos.y;
  }
  else
  {
    m_Position.y += velocity.y;
  }

  return result;
}

GameNetVec2 GameServerObjectBase::MoveCheckIntersectionDatabase(GameLogicContainer & game_container, const GameNetVec2 & vel, GameNetVal player_radius, GameNetVal move_threshold)
{
  GameNetVal one = GameNetVal(1);
  GameNetVal two = GameNetVal(2);

  auto & stage = game_container.GetStage();
  auto & collision = stage.GetIntersectionDatabase();

  auto result_velocity = vel;
  auto velocity = vel;

  Intersection<GameNetVal>::CollisionLine movement(m_Position, m_Position + velocity);
  IntersectionResult<GameNetVal> result;
  CollisionDatabaseData<GameNetVal> hit_line;

  if (collision.SweptCircleTest(movement, player_radius, result, hit_line, 0x01) == false)
  {
    m_Position += velocity;
  }
  else
  {
    if (result.m_T > move_threshold)
    {
      result.m_T -= move_threshold;

      auto actual_movement = velocity * result.m_T;
      m_Position += actual_movement;

      velocity -= actual_movement;
    }

    auto reflect_dp = IntersectionVecFuncs<GameNetVec2>::Dot(result.m_HitNormal, result_velocity);
    result_velocity -= result.m_HitNormal * reflect_dp * two;

    auto second_check_dp = IntersectionVecFuncs<GameNetVec2>::Dot(result.m_HitNormal, velocity);
    velocity -= result.m_HitNormal * second_check_dp * two;

    Intersection<GameNetVal>::CollisionLine new_movement(m_Position, m_Position + velocity);
    if (collision.SweptCircleTest(new_movement, player_radius, result, hit_line, 0x01) == false)
    {
      m_Position += velocity;
    }
  }

  return result_velocity;
}
