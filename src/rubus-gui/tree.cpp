#include "tree.hpp"

namespace rugui {

Tree::Tree() {
  root = (new Node{"root"})->set_color(SkColors::kTransparent);
}

Tree::~Tree() {
  root->delete_all_children();
  delete root;
}

auto Tree::init(Screen *screen) -> void {
  update_size(screen);
}

auto Tree::update_size(Screen *screen) -> void {
  root->style.width = {SizeMode::Self, (float)screen->width};
  root->style.height = {SizeMode::Self, (float)screen->height};
}

auto Tree::run_mouse_event(int mouse_x, int mouse_y) -> void {
  if (!is_mouse_button_enabled) {
    return;
  }

  const auto prev_node_under_mouse = node_under_mouse;

  root->dfs([&](Node *node) -> Node::Traverse {
    if (node->output.style.display_mode == DisplayMode::Collapsed) {
      return Node::Traverse::SkipChildren;
    }
    if (node->check_mouse_inside(mouse_x, mouse_y)) {
      node_under_mouse = node;
      node->run_mouse_enter_event(mouse_x, mouse_y);
    } else {
      node->run_mouse_leave_event(mouse_x, mouse_y);
    }
    return Node::Traverse::Continue;
  });

  if (prev_node_under_mouse != nullptr && !prev_node_under_mouse->check_mouse_inside(mouse_x, mouse_y)) {
    prev_node_under_mouse->run_mouse_leave_out_event(mouse_x, mouse_y);
  }

  if (node_under_mouse != nullptr) {
    node_under_mouse->run_mouse_over_event(mouse_x, mouse_y);
  }
}

auto Tree::run_mouse_leave_window_event(int mouse_x, int mouse_y) -> void {
  if (node_under_mouse != nullptr) {
    node_under_mouse->run_mouse_leave_out_event(mouse_x, mouse_y);
    node_under_mouse = nullptr;
  }
  is_mouse_down = false;
}

auto Tree::run_mouse_down_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  if (!is_mouse_button_enabled) {
    return;
  }
  is_mouse_down = true;

  node_mouse_down = node_under_mouse;
  if (node_under_mouse != nullptr) {
    node_under_mouse->run_mouse_down_event(button, mouse_x, mouse_y);
  }
}

auto Tree::run_mouse_up_event(MouseButton button, int mouse_x, int mouse_y) -> void {
  if (!is_mouse_button_enabled && !is_mouse_down) {
    return;
  }
  is_mouse_down = false;

  // mouse up
  if (node_under_mouse != nullptr) {
    node_under_mouse->run_mouse_up_event(button, mouse_x, mouse_y);
  }

  // mouse click out
  if (node_mouse_down != nullptr) {
    if (!node_mouse_down->check_mouse_inside(mouse_x, mouse_y) && node_mouse_down->is_mouse_down) {
      node_mouse_down->run_mouse_click_out_event(button, mouse_x, mouse_y);
    }
    node_mouse_down->is_mouse_down = false;
  }
  node_mouse_down = nullptr;
}

auto Tree::run_vscroll_event(int delta_y) -> void {
  if (!is_mouse_scroll_enabled) {
    return;
  }

  if (node_under_mouse != nullptr) {
    auto node = node_under_mouse;
    while (node != nullptr) {
      if (node->output.content_overflow.fHeight > 0) {
        const auto min_y = -node->output.content_overflow.fHeight;
        const auto max_y = 0.f;

        if (node->style.vscroll_amount == min_y && delta_y < 0) {
          node = node->parent;
          continue;
        }
        if (node->style.vscroll_amount == max_y && delta_y > 0) {
          node = node->parent;
          continue;
        }

        node->style.vscroll_amount = std::clamp(node->style.vscroll_amount + (float)delta_y, min_y, max_y);
        break;
      }
      node = node->parent;
    }
  }
}

} // namespace rugui
