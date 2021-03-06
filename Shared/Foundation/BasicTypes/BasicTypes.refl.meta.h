#pragma once

#include <StormRefl/StormReflMetaInfoBase.h>

#include "BasicTypes.refl.h"


template <>
struct StormReflTypeInfo<Vector2>
{
  using MyBase = void;
  static constexpr int fields_n = 2;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "Vector2"; }
  static constexpr auto GetNameHash() { return 0x29CA61A5; }
  static constexpr bool HasDefault() { return true; }
  static Vector2 & GetDefault() { static Vector2 def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<Vector2 *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const Vector2 *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<Vector2 *>(ptr);
    if(typeid(Vector2).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const Vector2 *>(ptr);
    if(typeid(Vector2).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<Vector2>::field_data_static<0>
{
  using member_type = int; // int
  static constexpr auto GetName() { return "x"; }
  static constexpr auto GetType() { return "int"; }
  static constexpr unsigned GetFieldNameHash() { return 0x8CDC1683; }
  static constexpr unsigned GetTypeNameHash() { return 0x1451DAB1; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &Vector2::x; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Vector2 *>(obj); return &ptr->x; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Vector2 *>(obj); return &ptr->x; }
};

template <typename Self>
struct StormReflTypeInfo<Vector2>::field_data<0, Self> : public StormReflTypeInfo<Vector2>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, int> & Get() { return self.x; }
  std::add_const_t<std::remove_reference_t<int>> & Get() const { return self.x; }
  void SetDefault() { self.x = StormReflTypeInfo<Vector2>::GetDefault().x; }
};

template <>
struct StormReflTypeInfo<Vector2>::field_data_static<1>
{
  using member_type = int; // int
  static constexpr auto GetName() { return "y"; }
  static constexpr auto GetType() { return "int"; }
  static constexpr unsigned GetFieldNameHash() { return 0xFBDB2615; }
  static constexpr unsigned GetTypeNameHash() { return 0x1451DAB1; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &Vector2::y; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Vector2 *>(obj); return &ptr->y; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Vector2 *>(obj); return &ptr->y; }
};

template <typename Self>
struct StormReflTypeInfo<Vector2>::field_data<1, Self> : public StormReflTypeInfo<Vector2>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, int> & Get() { return self.y; }
  std::add_const_t<std::remove_reference_t<int>> & Get() const { return self.y; }
  void SetDefault() { self.y = StormReflTypeInfo<Vector2>::GetDefault().y; }
};

template <>
struct StormReflTypeInfo<Line>
{
  using MyBase = void;
  static constexpr int fields_n = 2;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "Line"; }
  static constexpr auto GetNameHash() { return 0x71261BC8; }
  static constexpr bool HasDefault() { return true; }
  static Line & GetDefault() { static Line def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<Line *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const Line *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<Line *>(ptr);
    if(typeid(Line).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const Line *>(ptr);
    if(typeid(Line).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<Line>::field_data_static<0>
{
  using member_type = Vector2; // Vector2
  static constexpr auto GetName() { return "m_Start"; }
  static constexpr auto GetType() { return "Vector2"; }
  static constexpr unsigned GetFieldNameHash() { return 0x124FC5EF; }
  static constexpr unsigned GetTypeNameHash() { return 0x29CA61A5; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &Line::m_Start; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Line *>(obj); return &ptr->m_Start; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Line *>(obj); return &ptr->m_Start; }
};

template <typename Self>
struct StormReflTypeInfo<Line>::field_data<0, Self> : public StormReflTypeInfo<Line>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, Vector2> & Get() { return self.m_Start; }
  std::add_const_t<std::remove_reference_t<Vector2>> & Get() const { return self.m_Start; }
  void SetDefault() { self.m_Start = StormReflTypeInfo<Line>::GetDefault().m_Start; }
};

template <>
struct StormReflTypeInfo<Line>::field_data_static<1>
{
  using member_type = Vector2; // Vector2
  static constexpr auto GetName() { return "m_End"; }
  static constexpr auto GetType() { return "Vector2"; }
  static constexpr unsigned GetFieldNameHash() { return 0xF31EF756; }
  static constexpr unsigned GetTypeNameHash() { return 0x29CA61A5; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &Line::m_End; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Line *>(obj); return &ptr->m_End; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Line *>(obj); return &ptr->m_End; }
};

template <typename Self>
struct StormReflTypeInfo<Line>::field_data<1, Self> : public StormReflTypeInfo<Line>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, Vector2> & Get() { return self.m_End; }
  std::add_const_t<std::remove_reference_t<Vector2>> & Get() const { return self.m_End; }
  void SetDefault() { self.m_End = StormReflTypeInfo<Line>::GetDefault().m_End; }
};

template <>
struct StormReflTypeInfo<Box>
{
  using MyBase = void;
  static constexpr int fields_n = 2;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "Box"; }
  static constexpr auto GetNameHash() { return 0x30E40EDA; }
  static constexpr bool HasDefault() { return true; }
  static Box & GetDefault() { static Box def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<Box *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const Box *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<Box *>(ptr);
    if(typeid(Box).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const Box *>(ptr);
    if(typeid(Box).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<Box>::field_data_static<0>
{
  using member_type = Vector2; // Vector2
  static constexpr auto GetName() { return "m_Start"; }
  static constexpr auto GetType() { return "Vector2"; }
  static constexpr unsigned GetFieldNameHash() { return 0x124FC5EF; }
  static constexpr unsigned GetTypeNameHash() { return 0x29CA61A5; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &Box::m_Start; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Box *>(obj); return &ptr->m_Start; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Box *>(obj); return &ptr->m_Start; }
};

template <typename Self>
struct StormReflTypeInfo<Box>::field_data<0, Self> : public StormReflTypeInfo<Box>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, Vector2> & Get() { return self.m_Start; }
  std::add_const_t<std::remove_reference_t<Vector2>> & Get() const { return self.m_Start; }
  void SetDefault() { self.m_Start = StormReflTypeInfo<Box>::GetDefault().m_Start; }
};

template <>
struct StormReflTypeInfo<Box>::field_data_static<1>
{
  using member_type = Vector2; // Vector2
  static constexpr auto GetName() { return "m_End"; }
  static constexpr auto GetType() { return "Vector2"; }
  static constexpr unsigned GetFieldNameHash() { return 0xF31EF756; }
  static constexpr unsigned GetTypeNameHash() { return 0x29CA61A5; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &Box::m_End; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Box *>(obj); return &ptr->m_End; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Box *>(obj); return &ptr->m_End; }
};

template <typename Self>
struct StormReflTypeInfo<Box>::field_data<1, Self> : public StormReflTypeInfo<Box>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, Vector2> & Get() { return self.m_End; }
  std::add_const_t<std::remove_reference_t<Vector2>> & Get() const { return self.m_End; }
  void SetDefault() { self.m_End = StormReflTypeInfo<Box>::GetDefault().m_End; }
};

template <>
struct StormReflTypeInfo<Color>
{
  using MyBase = void;
  static constexpr int fields_n = 4;
  template <int N> struct field_data_static {};
  template <int N, typename Self> struct field_data {};
  template <int N> struct annotations { static constexpr int annotations_n = 0; template <int A> struct annoation { }; };
  static constexpr auto GetName() { return "Color"; }
  static constexpr auto GetNameHash() { return 0xA79767ED; }
  static constexpr bool HasDefault() { return true; }
  static Color & GetDefault() { static Color def; return def; }

  static void * CastFromTypeNameHash(uint32_t type_name_hash, void * ptr)
  {
    auto c = static_cast<Color *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeNameHash(uint32_t type_name_hash, const void * ptr)
  {
    auto c = static_cast<const Color *>(ptr);
    if(GetNameHash() == type_name_hash) return c;
    return nullptr;
  }

  static void * CastFromTypeIdHash(std::size_t type_id_hash, void * ptr)
  {
    auto c = static_cast<Color *>(ptr);
    if(typeid(Color).hash_code() == type_id_hash) return c;
    return nullptr;
  }

  static const void * CastFromTypeIdHash(std::size_t type_id_hash, const void * ptr)
  {
    auto c = static_cast<const Color *>(ptr);
    if(typeid(Color).hash_code() == type_id_hash) return c;
    return nullptr;
  }

};

template <>
struct StormReflTypeInfo<Color>::field_data_static<0>
{
  using member_type = float; // float
  static constexpr auto GetName() { return "r"; }
  static constexpr auto GetType() { return "float"; }
  static constexpr unsigned GetFieldNameHash() { return 0x6C09FF9D; }
  static constexpr unsigned GetTypeNameHash() { return 0xC9A55E95; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 0; }
  static constexpr auto GetMemberPtr() { return &Color::r; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Color *>(obj); return &ptr->r; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Color *>(obj); return &ptr->r; }
};

template <typename Self>
struct StormReflTypeInfo<Color>::field_data<0, Self> : public StormReflTypeInfo<Color>::field_data_static<0>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, float> & Get() { return self.r; }
  std::add_const_t<std::remove_reference_t<float>> & Get() const { return self.r; }
  void SetDefault() { self.r = StormReflTypeInfo<Color>::GetDefault().r; }
};

template <>
struct StormReflTypeInfo<Color>::field_data_static<1>
{
  using member_type = float; // float
  static constexpr auto GetName() { return "g"; }
  static constexpr auto GetType() { return "float"; }
  static constexpr unsigned GetFieldNameHash() { return 0x01D41B76; }
  static constexpr unsigned GetTypeNameHash() { return 0xC9A55E95; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 1; }
  static constexpr auto GetMemberPtr() { return &Color::g; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Color *>(obj); return &ptr->g; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Color *>(obj); return &ptr->g; }
};

template <typename Self>
struct StormReflTypeInfo<Color>::field_data<1, Self> : public StormReflTypeInfo<Color>::field_data_static<1>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, float> & Get() { return self.g; }
  std::add_const_t<std::remove_reference_t<float>> & Get() const { return self.g; }
  void SetDefault() { self.g = StormReflTypeInfo<Color>::GetDefault().g; }
};

template <>
struct StormReflTypeInfo<Color>::field_data_static<2>
{
  using member_type = float; // float
  static constexpr auto GetName() { return "b"; }
  static constexpr auto GetType() { return "float"; }
  static constexpr unsigned GetFieldNameHash() { return 0x71BEEFF9; }
  static constexpr unsigned GetTypeNameHash() { return 0xC9A55E95; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 2; }
  static constexpr auto GetMemberPtr() { return &Color::b; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Color *>(obj); return &ptr->b; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Color *>(obj); return &ptr->b; }
};

template <typename Self>
struct StormReflTypeInfo<Color>::field_data<2, Self> : public StormReflTypeInfo<Color>::field_data_static<2>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, float> & Get() { return self.b; }
  std::add_const_t<std::remove_reference_t<float>> & Get() const { return self.b; }
  void SetDefault() { self.b = StormReflTypeInfo<Color>::GetDefault().b; }
};

template <>
struct StormReflTypeInfo<Color>::field_data_static<3>
{
  using member_type = float; // float
  static constexpr auto GetName() { return "a"; }
  static constexpr auto GetType() { return "float"; }
  static constexpr unsigned GetFieldNameHash() { return 0xE8B7BE43; }
  static constexpr unsigned GetTypeNameHash() { return 0xC9A55E95; }
  static constexpr bool HasDefault() { return true; }
  static constexpr auto GetFieldIndex() { return 3; }
  static constexpr auto GetMemberPtr() { return &Color::a; }
  static void * GetFromParent(void * obj) { auto ptr = static_cast<Color *>(obj); return &ptr->a; }
  static const void * GetFromParentConst(const void * obj) { auto ptr = static_cast<const Color *>(obj); return &ptr->a; }
};

template <typename Self>
struct StormReflTypeInfo<Color>::field_data<3, Self> : public StormReflTypeInfo<Color>::field_data_static<3>
{
  Self & self;
  field_data(Self & self) : self(self) {}
  match_const_t<Self, float> & Get() { return self.a; }
  std::add_const_t<std::remove_reference_t<float>> & Get() const { return self.a; }
  void SetDefault() { self.a = StormReflTypeInfo<Color>::GetDefault().a; }
};

namespace StormReflFileInfo
{
  struct BasicTypes
  {
    static const int types_n = 4;
    template <int i> struct type_info { using type = void; };
  };

  template <>
  struct BasicTypes::type_info<0>
  {
    using type = ::Vector2;
  };

  template <>
  struct BasicTypes::type_info<1>
  {
    using type = ::Line;
  };

  template <>
  struct BasicTypes::type_info<2>
  {
    using type = ::Box;
  };

  template <>
  struct BasicTypes::type_info<3>
  {
    using type = ::Color;
  };

}

