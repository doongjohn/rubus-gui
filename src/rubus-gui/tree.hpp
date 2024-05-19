#pragma once

#include "screen.hpp"
#include "node.hpp"

namespace rugui {

struct Tree {
  Node *root = nullptr;

  bool is_mouse_button_enabled = true;
  bool is_mouse_scroll_enabled = true;
  bool is_mouse_down = false;

  Node *node_under_mouse = nullptr;
  Node *node_mouse_down = nullptr;

  Tree();
  ~Tree();

  auto init(Screen *screen) -> void;
  auto reset() -> void;
  auto set_size(Screen *screen) -> void;

  auto run_mouse_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_leave_window_event(int mouse_x, int mouse_y) -> void;
  auto run_mouse_down_event(MouseButton button, int mouse_x, int mouse_y) -> void;
  auto run_mouse_up_event(MouseButton button, int mouse_x, int mouse_y) -> void;

  auto run_vscroll_event(int delta_y) -> void;
};

} // namespace rugui
