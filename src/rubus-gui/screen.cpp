#include "screen.hpp"

#include <algorithm>

namespace rugui {

auto Screen::set_size(int width, int height) -> void {
  this->width = std::max(width, 1);
  this->height = std::max(height, 1);
}

} // namespace RUBUS_GUI_NAMESPACE
