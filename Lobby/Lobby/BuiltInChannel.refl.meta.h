#pragma once

#include <StormRefl/StormReflMetaInfoBase.h>

#include "BuiltInChannel.refl.h"
#include "SharedTypes.refl.meta.h"


template <>
struct StormReflTypeInfo<BuiltInChannelInfo>
{
  using MyBase = void;
  static constexpr int fields_n = 3;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "BuiltInChannelInfo"; }
  static constexpr auto GetNameHash() { return 0x357FEF68; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static BuiltInChannelInfo & GetDefault() { static BuiltInChannelInfo def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannelInfo *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannelInfo *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannelInfo *>(ptr);
    if(typeid(BuiltInChannelInfo).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannelInfo *>(ptr);
    if(typeid(BuiltInChannelInfo).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<0>
{
  using member_type = RString; // RString
  static constexpr auto GetName() { return "m_ChannelName"; }
  static constexpr auto GetType() { return "RString"; }
  static constexpr unsigned GetFieldNameHash() { return 0x0B8018E2; }
  static constexpr unsigned GetTypeNameHash() { return 0x01F631DC; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &BuiltInChannelInfo::m_ChannelName; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<BuiltInChannelInfo *>(obj); return &ptr->m_ChannelName; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const BuiltInChannelInfo *>(obj); return &ptr->m_ChannelName; }
};

template <typename Self>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data<0, Self> : public StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, RString> & Get() { return self.m_ChannelName; }
  std::add_const_t<std::remove_reference_t<RString>> & Get() const { return self.m_ChannelName; }
  void SetDefault() { self.m_ChannelName = StormReflTypeInfo<BuiltInChannelInfo>::GetDefault().m_ChannelName; }
};

template <>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<1>
{
  using member_type = RString; // RString
  static constexpr auto GetName() { return "m_Motd"; }
  static constexpr auto GetType() { return "RString"; }
  static constexpr unsigned GetFieldNameHash() { return 0x9492F073; }
  static constexpr unsigned GetTypeNameHash() { return 0x01F631DC; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &BuiltInChannelInfo::m_Motd; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<BuiltInChannelInfo *>(obj); return &ptr->m_Motd; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const BuiltInChannelInfo *>(obj); return &ptr->m_Motd; }
};

template <typename Self>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data<1, Self> : public StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, RString> & Get() { return self.m_Motd; }
  std::add_const_t<std::remove_reference_t<RString>> & Get() const { return self.m_Motd; }
  void SetDefault() { self.m_Motd = StormReflTypeInfo<BuiltInChannelInfo>::GetDefault().m_Motd; }
};

template <>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<2>
{
  using member_type = RInt; // RNumber<int>
  static constexpr auto GetName() { return "m_RequiredAdminLevel"; }
  static constexpr auto GetType() { return "RNumber<int>"; }
  static constexpr unsigned GetFieldNameHash() { return 0xFB2EAB49; }
  static constexpr unsigned GetTypeNameHash() { return 0x75C9DA09; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static constexpr auto GetFieldIndex() { return 2; }
  static constexpr auto GetMemberPtr() { return &BuiltInChannelInfo::m_RequiredAdminLevel; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<BuiltInChannelInfo *>(obj); return &ptr->m_RequiredAdminLevel; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const BuiltInChannelInfo *>(obj); return &ptr->m_RequiredAdminLevel; }
};

template <typename Self>
struct StormReflTypeInfo<BuiltInChannelInfo>::field_data<2, Self> : public StormReflTypeInfo<BuiltInChannelInfo>::field_data_static<2>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, RInt> & Get() { return self.m_RequiredAdminLevel; }
  std::add_const_t<std::remove_reference_t<RInt>> & Get() const { return self.m_RequiredAdminLevel; }
  void SetDefault() { self.m_RequiredAdminLevel = StormReflTypeInfo<BuiltInChannelInfo>::GetDefault().m_RequiredAdminLevel; }
};

template <>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>
{
  using MyBase = void;
  static constexpr int fields_n = 2;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "BuiltInChannelDatabaseObject"; }
  static constexpr auto GetNameHash() { return 0x12592DE8; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static BuiltInChannelDatabaseObject & GetDefault() { static BuiltInChannelDatabaseObject def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannelDatabaseObject *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannelDatabaseObject *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannelDatabaseObject *>(ptr);
    if(typeid(BuiltInChannelDatabaseObject).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannelDatabaseObject *>(ptr);
    if(typeid(BuiltInChannelDatabaseObject).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data_static<0>
{
  using member_type = RString; // RString
  static constexpr auto GetName() { return "m_ChannelNameLower"; }
  static constexpr auto GetType() { return "RString"; }
  static constexpr unsigned GetFieldNameHash() { return 0x0A237DA3; }
  static constexpr unsigned GetTypeNameHash() { return 0x01F631DC; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &BuiltInChannelDatabaseObject::m_ChannelNameLower; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<BuiltInChannelDatabaseObject *>(obj); return &ptr->m_ChannelNameLower; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const BuiltInChannelDatabaseObject *>(obj); return &ptr->m_ChannelNameLower; }
};

template <typename Self>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data<0, Self> : public StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, RString> & Get() { return self.m_ChannelNameLower; }
  std::add_const_t<std::remove_reference_t<RString>> & Get() const { return self.m_ChannelNameLower; }
  void SetDefault() { self.m_ChannelNameLower = StormReflTypeInfo<BuiltInChannelDatabaseObject>::GetDefault().m_ChannelNameLower; }
};

template <>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::annotations<0>
{
  static constexpr int annotations_n = 1;
  template <int A> struct annoation { };
};

template <>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::annotations<0>::annoation<0>
{
  static constexpr const char * GetAnnotation() { return "UniqueIndex"; }
  static constexpr uint32_t GetAnnotationHash() { return 0x5C838CC5; }
};

template <>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data_static<1>
{
  using member_type = BuiltInChannelInfo; // BuiltInChannelInfo
  static constexpr auto GetName() { return "m_ChannelInfo"; }
  static constexpr auto GetType() { return "BuiltInChannelInfo"; }
  static constexpr unsigned GetFieldNameHash() { return 0x9E2A57B3; }
  static constexpr unsigned GetTypeNameHash() { return 0x357FEF68; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return true; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &BuiltInChannelDatabaseObject::m_ChannelInfo; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<BuiltInChannelDatabaseObject *>(obj); return &ptr->m_ChannelInfo; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const BuiltInChannelDatabaseObject *>(obj); return &ptr->m_ChannelInfo; }
};

template <typename Self>
struct StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data<1, Self> : public StormReflTypeInfo<BuiltInChannelDatabaseObject>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, BuiltInChannelInfo> & Get() { return self.m_ChannelInfo; }
  std::add_const_t<std::remove_reference_t<BuiltInChannelInfo>> & Get() const { return self.m_ChannelInfo; }
  void SetDefault() { self.m_ChannelInfo = StormReflTypeInfo<BuiltInChannelDatabaseObject>::GetDefault().m_ChannelInfo; }
};

template <>
struct StormReflTypeInfo<BuiltInChannel>
{
  using MyBase = void;
  static constexpr int fields_n = 0;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "BuiltInChannel"; }
  static constexpr auto GetNameHash() { return 0x81589DF8; }
<<<<<<< HEAD
=======
  static constexpr bool HasDefault() { return false; }
>>>>>>> a49d5fa0cf25199154acded458b9a5829dad762c

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannel *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannel *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<BuiltInChannel *>(ptr);
    if(typeid(BuiltInChannel).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const BuiltInChannel *>(ptr);
    if(typeid(BuiltInChannel).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflFuncInfo<BuiltInChannel>
{
  using MyBase = void;
  static constexpr int funcs_n = 2;
  template <int N> struct func_data_static {};
};

template <>
struct StormReflFuncInfo<BuiltInChannel>::func_data_static<0>
{
  using func_ptr_type = void (BuiltInChannel::*)(DDSResponder &);
  using return_type = void;
  static constexpr int params_n = 1;
  static constexpr auto GetName() { return "GetChannelName"; }
  static constexpr auto GetReturnType() { return "void"; }
  static constexpr unsigned GetFunctionNameHash() { return 0xC29BD60D; }
  static constexpr unsigned GetReturnTypeNameHash() { return 0xD27BD9EE; }
  static constexpr auto GetFunctionIndex() { return 0; }
  static constexpr func_ptr_type GetFunctionPtr() { return &BuiltInChannel::GetChannelName; }
  template <int i>
  struct param_info { };
};

template <>
struct StormReflFuncInfo<BuiltInChannel>::func_data_static<0>::param_info<0>
{
  using param_type = DDSResponder &;
  static constexpr auto GetName() { return "responder"; }
  static constexpr auto GetType() { return "DDSResponder &"; }
  static constexpr unsigned GetNameHash() { return 0x5F311AF7; }
  static constexpr unsigned GetTypeNameHash() { return 0xF2AB2E46; }
};

template <>
struct StormReflFuncInfo<BuiltInChannel>::func_data_static<1>
{
  using func_ptr_type = void (BuiltInChannel::*)(std::string);
  using return_type = void;
  static constexpr int params_n = 1;
  static constexpr auto GetName() { return "UpdateMotd"; }
  static constexpr auto GetReturnType() { return "void"; }
  static constexpr unsigned GetFunctionNameHash() { return 0xDF6CAD1B; }
  static constexpr unsigned GetReturnTypeNameHash() { return 0xD27BD9EE; }
  static constexpr auto GetFunctionIndex() { return 1; }
  static constexpr func_ptr_type GetFunctionPtr() { return &BuiltInChannel::UpdateMotd; }
  template <int i>
  struct param_info { };
};

template <>
struct StormReflFuncInfo<BuiltInChannel>::func_data_static<1>::param_info<0>
{
  using param_type = std::string;
  static constexpr auto GetName() { return "motd"; }
  static constexpr auto GetType() { return "std::string"; }
  static constexpr unsigned GetNameHash() { return 0xAA0F656C; }
  static constexpr unsigned GetTypeNameHash() { return 0xC508D6F7; }
};

namespace StormReflFileInfo
{
  struct BuiltInChannel
  {
    static const int types_n = 3;
    template <int i> struct type_info { using type = void; };
  };

  template <>
  struct BuiltInChannel::type_info<0>
  {
    using type = ::BuiltInChannelInfo;
  };

  template <>
  struct BuiltInChannel::type_info<1>
  {
    using type = ::BuiltInChannelDatabaseObject;
  };

  template <>
  struct BuiltInChannel::type_info<2>
  {
    using type = ::BuiltInChannel;
  };

}

