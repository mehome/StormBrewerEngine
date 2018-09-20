#pragma once

#include <StormRefl/StormReflMetaInfoBase.h>

#include "DealDamageAnimationEvent.refl.h"
#include "Game/GameplayEvents/DamageType.refl.meta.h"
#include "Game/ServerObjects/CharacterFacing.refl.meta.h"
#include "Runtime/SpriteBase/SpriteAnimationEventDef.refl.meta.h"


template <>
struct StormReflTypeInfo<DealDamageAnimationEvent>
{
  using MyBase = SpriteAnimationEventDataBase;
  static constexpr int fields_n = 3 + StormReflTypeInfo<MyBase>::fields_n;
  template <int N> struct field_data_static : public StormReflTypeInfo<MyBase>::field_data_static<N> {};
  template <int N, typename Self> struct field_data : public StormReflTypeInfo<MyBase>::field_data<N, match_const_t<Self, MyBase>>
  {
    field_data(Self & self) : StormReflTypeInfo<MyBase>::field_data<N, match_const_t<Self, MyBase>>(self) {}
  };
  template <int N> struct annotations : public StormReflTypeInfo<MyBase>::annotations<N> {};
  static constexpr auto GetName() { return "DealDamageAnimationEvent"; }
  static constexpr auto GetNameHash() { return 0xCA028F12; }
  static DealDamageAnimationEvent & GetDefault() { static DealDamageAnimationEvent def; return def; }
};

template <>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<0 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  using member_type = RInt; // RNumber<int>
  static constexpr auto GetName() { return "m_Amount"; }
  static constexpr auto GetType() { return "RNumber<int>"; }
  static constexpr unsigned GetFieldNameHash() { return 0xC39E278A; }
  static constexpr unsigned GetTypeNameHash() { return 0x75C9DA09; }
  static constexpr auto GetFieldIndex() { return 0 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n; }
  static constexpr auto GetMemberPtr() { return &DealDamageAnimationEvent::m_Amount; }
};

template <typename Self>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data<0 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n, Self> : public StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<0 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, RInt> & Get() { return self.m_Amount; }
  std::add_const_t<std::remove_reference_t<RInt>> & Get() const { return self.m_Amount; }
  void SetDefault() { self.m_Amount = StormReflTypeInfo<DealDamageAnimationEvent>::GetDefault().m_Amount; }
};

template <>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<1 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  using member_type = REnum<CharacterFacing>; // REnum<CharacterFacing>
  static constexpr auto GetName() { return "m_Direction"; }
  static constexpr auto GetType() { return "REnum<CharacterFacing>"; }
  static constexpr unsigned GetFieldNameHash() { return 0xE26AF0A8; }
  static constexpr unsigned GetTypeNameHash() { return 0xBDF26CA6; }
  static constexpr auto GetFieldIndex() { return 1 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n; }
  static constexpr auto GetMemberPtr() { return &DealDamageAnimationEvent::m_Direction; }
};

template <typename Self>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data<1 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n, Self> : public StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<1 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, REnum<CharacterFacing>> & Get() { return self.m_Direction; }
  std::add_const_t<std::remove_reference_t<REnum<CharacterFacing>>> & Get() const { return self.m_Direction; }
  void SetDefault() { self.m_Direction = StormReflTypeInfo<DealDamageAnimationEvent>::GetDefault().m_Direction; }
};

template <>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<2 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  using member_type = REnum<DamageType>; // REnum<DamageType>
  static constexpr auto GetName() { return "m_DamageType"; }
  static constexpr auto GetType() { return "REnum<DamageType>"; }
  static constexpr unsigned GetFieldNameHash() { return 0xD583A2B9; }
  static constexpr unsigned GetTypeNameHash() { return 0x33D1B477; }
  static constexpr auto GetFieldIndex() { return 2 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n; }
  static constexpr auto GetMemberPtr() { return &DealDamageAnimationEvent::m_DamageType; }
};

template <typename Self>
struct StormReflTypeInfo<DealDamageAnimationEvent>::field_data<2 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n, Self> : public StormReflTypeInfo<DealDamageAnimationEvent>::field_data_static<2 + StormReflTypeInfo<SpriteAnimationEventDataBase>::fields_n>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, REnum<DamageType>> & Get() { return self.m_DamageType; }
  std::add_const_t<std::remove_reference_t<REnum<DamageType>>> & Get() const { return self.m_DamageType; }
  void SetDefault() { self.m_DamageType = StormReflTypeInfo<DealDamageAnimationEvent>::GetDefault().m_DamageType; }
};

namespace StormReflFileInfo
{
  struct DealDamageAnimationEvent
  {
    static const int types_n = 1;
    template <int i> struct type_info { using type = void; };
  };

  template <>
  struct DealDamageAnimationEvent::type_info<0>
  {
    using type = ::DealDamageAnimationEvent;
  };

}
