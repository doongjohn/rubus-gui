#pragma once

#include <include/core/SkColor.h>

namespace rugui {

enum class MouseButton {
  Left,
  Right,
  Middle,
};

auto color_add(SkColor4f a, SkColor4f b) -> SkColor4f;
auto color_sub(SkColor4f a, SkColor4f b) -> SkColor4f;

} // namespace gui
