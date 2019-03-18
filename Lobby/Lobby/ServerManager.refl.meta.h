#pragma once

#include <StormRefl/StormReflMetaInfoBase.h>

#include "ServerManager.refl.h"
#include "GooglePlatform.refl.meta.h"


template <>
struct StormReflTypeInfo<ServerManager>
{
  using MyBase = void;
  static constexpr int fields_n = 0;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "ServerManager"; }
  static constexpr auto GetNameHash() { return 0x06A88995; }
  static constexpr bool HasDefault() { return false; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<ServerManager *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const ServerManager *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<ServerManager *>(ptr);
    if(typeid(ServerManager).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const ServerManager *>(ptr);
    if(typeid(ServerManager).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflFuncInfo<ServerManager>
{
  using MyBase = void;
  static constexpr int funcs_n = 2;
  template <int N> struct func_data_static {};
};

template <>
struct StormReflFuncInfo<ServerManager>::func_data_static<0>
{
  using func_ptr_type = void (ServerManager::*)();
  using return_type = void;
  static constexpr int params_n = 0;
  static constexpr auto GetName() { return "RequestNewToken"; }
  static constexpr auto GetReturnType() { return "void"; }
  static constexpr unsigned GetFunctionNameHash() { return 0x18E1851B; }
  static constexpr unsigned GetReturnTypeNameHash() { return 0xD27BD9EE; }
  static constexpr auto GetFunctionIndex() { return 0; }
  static constexpr func_ptr_type GetFunctionPtr() { return &ServerManager::RequestNewToken; }
  template <int i>
  struct param_info { };
};

template <>
struct StormReflFuncInfo<ServerManager>::func_data_static<1>
{
  using func_ptr_type = void (ServerManager::*)(bool, std::string, std::string);
  using return_type = void;
  static constexpr int params_n = 3;
  static constexpr auto GetName() { return "HandleTokenResponse"; }
  static constexpr auto GetReturnType() { return "void"; }
  static constexpr unsigned GetFunctionNameHash() { return 0xA77CB98C; }
  static constexpr unsigned GetReturnTypeNameHash() { return 0xD27BD9EE; }
  static constexpr auto GetFunctionIndex() { return 1; }
  static constexpr func_ptr_type GetFunctionPtr() { return &ServerManager::HandleTokenResponse; }
  template <int i>
  struct param_info { };
};

template <>
struct StormReflFuncInfo<ServerManager>::func_data_static<1>::param_info<0>
{
  using param_type = bool;
  static constexpr auto GetName() { return "success"; }
  static constexpr auto GetType() { return "bool"; }
  static constexpr unsigned GetNameHash() { return 0x6F00DFB2; }
  static constexpr unsigned GetTypeNameHash() { return 0x55813692; }
};

template <>
struct StormReflFuncInfo<ServerManager>::func_data_static<1>::param_info<1>
{
  using param_type = std::string;
  static constexpr auto GetName() { return "body"; }
  static constexpr auto GetType() { return "std::string"; }
  static constexpr unsigned GetNameHash() { return 0xDBA80BB2; }
  static constexpr unsigned GetTypeNameHash() { return 0xC508D6F7; }
};

template <>
struct StormReflFuncInfo<ServerManager>::func_data_static<1>::param_info<2>
{
  using param_type = std::string;
  static constexpr auto GetName() { return "headers"; }
  static constexpr auto GetType() { return "std::string"; }
  static constexpr unsigned GetNameHash() { return 0xF7034F85; }
  static constexpr unsigned GetTypeNameHash() { return 0xC508D6F7; }
};

namespace StormReflFileInfo
{
  struct ServerManager
  {
    static const int types_n = 1;
    template <int i> struct type_info { using type = void; };
  };

  template <>
  struct ServerManager::type_info<0>
  {
    using type = ::ServerManager;
  };

}
