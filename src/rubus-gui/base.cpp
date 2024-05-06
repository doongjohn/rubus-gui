#include "base.hpp"

#include <algorithm>

namespace rugui {

auto color_add(SkColor4f a, SkColor4f b) -> SkColor4f {
  auto result = a;
  result.fA = std::clamp(result.fA + b.fA, 0.f, 1.f);
  result.fR = std::clamp(result.fR + b.fR, 0.f, 1.f);
  result.fG = std::clamp(result.fG + b.fG, 0.f, 1.f);
  result.fB = std::clamp(result.fB + b.fB, 0.f, 1.f);
  return result;
}

auto color_sub(SkColor4f a, SkColor4f b) -> SkColor4f {
  auto result = a;
  result.fA = std::clamp(result.fA - b.fA, 0.f, 1.f);
  result.fR = std::clamp(result.fR - b.fR, 0.f, 1.f);
  result.fG = std::clamp(result.fG - b.fG, 0.f, 1.f);
  result.fB = std::clamp(result.fB - b.fB, 0.f, 1.f);
  return result;
}

} // namespace gui
