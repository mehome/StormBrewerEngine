#pragma once

#include <glm/glm.hpp>

static const float kPi = 3.14159265359f;
static const float kHalfPi = 1.57079632679f;
static const float k2Pi = 6.28318530718f;
static const float kSqrt2 = 1.41421356237f;

using zstr = gsl::zstring<>;
using czstr = gsl::czstring<>;
using zwstr = gsl::wzstring<>;
using czwstr = gsl::cwzstring<>;

using RenderVec2 = glm::vec2;
using RenderVec3 = glm::vec3;
using RenderVec4 = glm::vec4;
using RenderMat3 = glm::mat3x4;
using RenderMat4 = glm::mat4x4;

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;
using Mat3f = glm::mat3x4;
using Mat4f = glm::mat4x4;

static inline Vector2f SinCosf(float angle)
{
  return Vector2f{ cos(angle), sin(angle) };
}

static inline int RoundDownInt(int x, int mod)
{
  int m = x % mod;
  return m > 0 ? x - m : x - (mod + m);
};

static inline int RoundUpInt(int x, int mod)
{
  int m = x % mod;
  return m == 0 ? x : (m > 0 ? x - m : x - (mod + m)) + mod;
};

template <typename ValueType>
ValueType Lerp(ValueType a, ValueType b, ValueType i)
{
  return a + (b - a) * i;
}

template <typename ValueType>
ValueType Normalize(ValueType a, ValueType b, ValueType i)
{
  return (i - a) / (b - a);
}
