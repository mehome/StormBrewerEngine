#pragma once

#include "Foundation/BasicTypes/BasicTypes.refl.h"


inline Vector2 SelectVectorXY(const Vector2 & v1, const Vector2 & v2, uint32_t mask)
{
  return Vector2( (mask & 0b01) ? v1.x : v2.x, (mask & 0b10) ? v1.y : v2.y );
}

inline Vector2 MaxVectorXY(const Vector2 & v1, const Vector2 & v2)
{
  return Vector2(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}

inline Vector2 MinVectorXY(const Vector2 & v1, const Vector2 & v2)
{
  return Vector2(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

inline RenderVec2 SelectVectorXY(const RenderVec2 & v1, const RenderVec2 & v2, uint32_t mask)
{
  return RenderVec2{ (mask & 0b01) ? v1.x : v2.x, (mask & 0b10) ? v1.y : v2.y };
}

inline RenderVec2 MaxVectorXY(const RenderVec2 & v1, const RenderVec2 & v2)
{
  return RenderVec2{ std::max(v1.x, v2.x), std::max(v1.y, v2.y) };
}

inline RenderVec2 MinVectorXY(const RenderVec2 & v1, const RenderVec2 & v2)
{
  return RenderVec2{ std::min(v1.x, v2.x), std::min(v1.y, v2.y) };
}

inline bool BoxIntersect(const Box & b1, const Box & b2)
{
  return !(
    b1.m_Start.x > b2.m_End.x |
    b1.m_Start.y > b2.m_End.y |
    b1.m_End.x < b2.m_Start.x |
    b1.m_End.y < b2.m_Start.y);
}

inline bool PointInBox(const Box & b, const Vector2 & p)
{
  return !(
    b.m_Start.x > p.x |
    b.m_Start.y > p.y |
    b.m_End.x < p.x |
    b.m_End.y < p.y);
}

inline Optional<Box> ClipBox(const Box & box, const Box & bounds)
{
  if (!BoxIntersect(box, bounds))
  {
    return Optional<Box>();
  }

  Box result;
  result.m_Start = MaxVectorXY(box.m_Start, bounds.m_Start);
  result.m_End = MinVectorXY(box.m_End, bounds.m_End);
  return result;
}

inline Box MakeBoxFromStartAndSize(const Vector2 & start, const Vector2 & size)
{
  return Box{ start, start + size };
}