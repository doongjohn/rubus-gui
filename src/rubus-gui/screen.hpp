#pragma once

namespace rugui {

struct Screen {
  int width;
  int height;

  Screen() = default;
  Screen(int width, int height) : width{width}, height{height} {}

  auto update_size(int width, int height) -> void;
};

} // namespace gui
