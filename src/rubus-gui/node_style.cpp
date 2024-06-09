#include "node_style.hpp"
#include "node.hpp"
#include <iostream>

namespace rugui {

auto NodeStyle::set_display_mode(DisplayMode mode) -> NodeStyle & {
  this->display_mode = mode;
  return *this;
}

auto NodeStyle::set_local_transform(SkMatrix transform) -> NodeStyle & {
  this->transform_mode = TransformMode::Local;
  this->transform = transform;
  return *this;
}

auto NodeStyle::set_screen_transform(SkMatrix transform) -> NodeStyle & {
  this->transform_mode = TransformMode::Screen;
  this->transform = transform;
  return *this;
}

auto NodeStyle::set_color(SkColor4f color) -> NodeStyle & {
  this->color = color;
  return *this;
}

auto NodeStyle::set_font_size(float size) -> NodeStyle & {
  this->font_size = size;
  return *this;
}

auto NodeStyle::set_image(sk_sp<SkImage> image) -> NodeStyle & {
  this->image = image;
  return *this;
}

auto NodeStyle::set_image_sampling(SkSamplingOptions image_sampling) -> NodeStyle & {
  this->image_sampling = image_sampling;
  return *this;
}

auto NodeStyle::set_vscroll(bool value) -> NodeStyle & {
  this->is_vscroll_enabled = value;
  return *this;
}

auto NodeStyle::set_hscroll(bool value) -> NodeStyle & {
  this->is_hscroll_enabled = value;
  return *this;
}

auto NodeStyle::set_clip_children(bool value) -> NodeStyle & {
  this->is_clip_enabled = value;
  return *this;
}

auto NodeStyle::set_width(Size width) -> NodeStyle & {
  this->width = width;
  return *this;
}

auto NodeStyle::set_height(Size height) -> NodeStyle & {
  this->height = height;
  return *this;
}

auto NodeStyle::set_border_radius(float value) -> NodeStyle & {
  this->border_radius_tl = value;
  this->border_radius_tr = value;
  this->border_radius_br = value;
  this->border_radius_bl = value;
  return *this;
}

auto NodeStyle::set_border_radius_tl(float value) -> NodeStyle & {
  this->border_radius_tl = value;
  return *this;
}

auto NodeStyle::set_border_radius_tr(float value) -> NodeStyle & {
  this->border_radius_tr = value;
  return *this;
}

auto NodeStyle::set_border_radius_br(float value) -> NodeStyle & {
  this->border_radius_br = value;
  return *this;
}

auto NodeStyle::set_border_radius_bl(float value) -> NodeStyle & {
  this->border_radius_bl = value;
  return *this;
}

auto NodeStyle::set_margin(float value) -> NodeStyle & {
  this->margin_t = value;
  this->margin_b = value;
  this->margin_r = value;
  this->margin_l = value;
  return *this;
}

auto NodeStyle::set_margin_row(float value) -> NodeStyle & {
  this->margin_r = value;
  this->margin_l = value;
  return *this;
}

auto NodeStyle::set_margin_col(float value) -> NodeStyle & {
  this->margin_t = value;
  this->margin_b = value;
  return *this;
}

auto NodeStyle::set_margin_t(float value) -> NodeStyle & {
  this->margin_t = value;
  return *this;
}

auto NodeStyle::set_margin_b(float value) -> NodeStyle & {
  this->margin_b = value;
  return *this;
}

auto NodeStyle::set_margin_r(float value) -> NodeStyle & {
  this->margin_r = value;
  return *this;
}

auto NodeStyle::set_margin_l(float value) -> NodeStyle & {
  this->margin_l = value;
  return *this;
}

auto NodeStyle::set_padding(float value) -> NodeStyle & {
  this->padding_t = value;
  this->padding_b = value;
  this->padding_r = value;
  this->padding_l = value;
  return *this;
}

auto NodeStyle::set_padding_row(float value) -> NodeStyle & {
  this->padding_r = value;
  this->padding_l = value;
  return *this;
}

auto NodeStyle::set_padding_col(float value) -> NodeStyle & {
  this->padding_t = value;
  this->padding_b = value;
  return *this;
}

auto NodeStyle::set_padding_t(float value) -> NodeStyle & {
  this->padding_t = value;
  return *this;
}

auto NodeStyle::set_padding_b(float value) -> NodeStyle & {
  this->padding_b = value;
  return *this;
}

auto NodeStyle::set_padding_r(float value) -> NodeStyle & {
  this->padding_r = value;
  return *this;
}

auto NodeStyle::set_padding_l(float value) -> NodeStyle & {
  this->padding_l = value;
  return *this;
}

auto NodeStyle::set_flex_dir(FlexDir flex_dir) -> NodeStyle & {
  this->flex_dir = flex_dir;
  return *this;
}

auto NodeStyle::set_flex_wrap(FlexWrap flex_wrap) -> NodeStyle & {
  this->flex_wrap = flex_wrap;
  return *this;
}

auto NodeStyle::set_flex_align(FlexAlign align) -> NodeStyle & {
  this->flex_align = align;
  return *this;
}

auto NodeStyle::set_flex_items_align(FlexAlign align) -> NodeStyle & {
  this->flex_items_align = align;
  return *this;
}

auto NodeStyle::set_flex_content_align(FlexAlign align) -> NodeStyle & {
  this->flex_content_align = align;
  return *this;
}

auto NodeStyle::set_flex_self_align(FlexAlign align) -> NodeStyle & {
  this->flex_self_align = align;
  return *this;
}

auto UiNodeOutput::get_rect_pos() -> SkPoint {
  return {pos.fX + style.margin_l, pos.fY + style.margin_t};
}

auto UiNodeOutput::get_content_area() -> SkSize {
  return SkSize{
    rect_size.fWidth - style.padding_l - style.padding_r,
    rect_size.fHeight - style.padding_t - style.padding_b,
  };
}

auto UiNodeOutput::get_margin_row() -> float {
  return style.margin_l + style.margin_r;
}

auto UiNodeOutput::get_margin_col() -> float {
  return style.margin_t + style.margin_b;
}

auto UiNodeOutput::get_padding_row() -> float {
  return style.padding_l + style.padding_r;
}

auto UiNodeOutput::get_padding_col() -> float {
  return style.padding_t + style.padding_b;
}

auto UiNodeOutput::calculate_flex_lines(Node *node) -> void {
  node->output.flex_lines.emplace_back();
  auto &content_width = node->output.content_size.fWidth;
  auto &content_height = node->output.content_size.fHeight;

  switch (node->output.style.flex_wrap) {
  case FlexWrap::NoWrap: {
    auto &line = node->output.flex_lines.back();
    switch (node->type) {
    case Node::Type::Rect: {
      line.items = node->children;
      for (const auto child : node->children) {
        switch (node->output.style.flex_dir) {
        case FlexDir::Row:
          line.width += child->output.layout_size.fWidth;
          if (line.height < child->output.layout_size.fHeight) {
            line.height = child->output.layout_size.fHeight;
          }
          break;
        case FlexDir::Col:
          if (line.width < child->output.layout_size.fWidth) {
            line.width = child->output.layout_size.fWidth;
          }
          line.height += child->output.layout_size.fHeight;
          break;
        }
      }

      content_width = line.width;
      content_height = line.height;

      if (node->output.style.width.mode == SizeMode::FitContent) {
        const auto margin_row = node->output.get_margin_row();
        const auto padding_row = node->output.get_padding_row();
        node->output.layout_size.fWidth = content_width + margin_row + padding_row;
        node->output.rect_size.fWidth = content_width + padding_row;
      }
      if (node->output.style.height.mode == SizeMode::FitContent) {
        const auto margin_col = node->output.get_margin_col();
        const auto padding_col = node->output.get_padding_col();
        node->output.layout_size.fHeight = content_height + margin_col + padding_col;
        node->output.rect_size.fHeight = content_height + padding_col;
      }
    } break;
    case Node::Type::Text: {
      node->paragraph->layout(node->output.get_content_area().fWidth);
      content_width = node->paragraph->getLongestLine();
      content_height = node->paragraph->getHeight();

      if (node->output.style.width.mode == SizeMode::FitContent) {
        const auto margin_row = node->output.get_margin_row();
        node->output.layout_size.fWidth = content_width + margin_row;
        node->output.rect_size.fWidth = content_width;
      }
      if (node->output.style.height.mode == SizeMode::FitContent) {
        const auto margin_col = node->output.get_margin_col();
        node->output.layout_size.fHeight = content_height + margin_col;
        node->output.rect_size.fHeight = content_height;
      }

      line.width = node->output.layout_size.fWidth;
      line.height = node->output.layout_size.fHeight;
    } break;
    }
  } break;
  case FlexWrap::Wrap: {
    switch (node->type) {
    case Node::Type::Rect: {
      content_width = 0.f;
      content_height = 0.f;

      auto line_width = 0.f;
      auto line_height = 0.f;
      auto line_start = std::size_t{};
      auto is_overflowed = false;

      for (auto i = std::size_t{}; i < node->children.size(); ++i) {
        const auto &child = node->children[i];
        auto &line = node->output.flex_lines.back();

        // check overflow
        switch (node->output.style.flex_dir) {
        case FlexDir::Row:
          if (line_width + child->output.layout_size.fWidth > node->output.get_content_area().fWidth) {
            is_overflowed = true;
          }
          break;
        case FlexDir::Col:
          if (line_height + child->output.layout_size.fHeight > node->output.get_content_area().fHeight) {
            is_overflowed = true;
          }
          break;
        }

        if (is_overflowed) {
          is_overflowed = false;
          line.items = std::span{node->children.data() + line_start, node->children.data() + i};

          switch (node->output.style.flex_dir) {
          case FlexDir::Row:
            if (content_width < line_width) {
              content_width = line_width;
            }
            content_height += line_height;
            line.width = line_width;
            line.height = line_height;
            break;
          case FlexDir::Col:
            content_width += line_width;
            if (content_height < line_height) {
              content_height = line_height;
            }
            line.width = line_width;
            line.height = line_height;
            break;
          }

          line_width = 0;
          line_height = 0;
          line_start = i;
          node->output.flex_lines.emplace_back();
        }

        switch (node->output.style.flex_dir) {
        case FlexDir::Row:
          line_width += child->output.layout_size.fWidth;
          if (line_height < child->output.layout_size.fHeight) {
            line_height = child->output.layout_size.fHeight;
          }
          break;
        case FlexDir::Col:
          if (line_width < child->output.layout_size.fWidth) {
            line_width = child->output.layout_size.fWidth;
          }
          line_height += child->output.layout_size.fHeight;
          break;
        }

        if (i == node->children.size() - 1) {
          if (!node->output.flex_lines.back().items.empty()) {
            node->output.flex_lines.emplace_back();
          }
          auto &line = node->output.flex_lines.back();
          line.items = std::span{node->children.data() + line_start, node->children.data() + node->children.size()};

          switch (node->output.style.flex_dir) {
          case FlexDir::Row:
            if (content_width < line_width) {
              content_width = line_width;
            }
            content_height += line_height;
            line.width = line_width;
            line.height = line_height;
            break;
          case FlexDir::Col:
            content_width += line_width;
            if (content_height < line_height) {
              content_height = line_height;
            }
            line.width = line_width;
            line.height = line_height;
            break;
          }
        }
      }
    } break;
    case Node::Type::Text: {
      auto content_area_width = node->output.get_content_area().fWidth;
      if (node->parent->output.style.width.mode == SizeMode::FitContent) {
        content_area_width = node->parent->output.max_content_area.fWidth - node->output.get_margin_row();
      }

      if (content_width > content_area_width) {
        node->paragraph->layout(content_area_width);
        content_width = node->paragraph->getLongestLine();
        content_height = node->paragraph->getHeight();
      }

      if (node->output.style.width.mode == SizeMode::FitContent) {
        const auto margin_row = node->output.get_margin_row();
        node->output.layout_size.fWidth = content_width + margin_row;
        node->output.rect_size.fWidth = content_width;
      }
      if (node->output.style.height.mode == SizeMode::FitContent) {
        const auto margin_col = node->output.get_margin_col();
        node->output.layout_size.fHeight = content_height + margin_col;
        node->output.rect_size.fHeight = content_height;
      }
    } break;
    }
  } break;
  }
}

} // namespace rugui
