#include "../include/layout_vertical.hpp"
using namespace duskland::tui;
layout_vertical::layout_vertical() { get_attribute().xoverflow = style::FIXED; }
void layout_vertical::on_update() {
  auto offset = 0;
  auto &current_attr = get_attribute();
  current_attr.size.height = 0;
  current_attr.size.width = 0;
  for (auto &c : get_children()) {
    auto &attr = c->get_attribute();
    if (attr.position == style::RELATIVE) {
      attr.offset.y = offset;
      attr.offset.x = 0;
      auto rc = c->get_bound_rect();
      offset += rc.height;
      current_attr.size.height += rc.height;
      if (current_attr.size.width < rc.width) {
        current_attr.size.width = rc.width;
      }
    }
  }
}