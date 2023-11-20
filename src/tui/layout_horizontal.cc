#include "tui/layout_horizontal.hpp"
using namespace duskland::tui;
layout_horizontal::layout_horizontal() {
  get_attribute().yoverflow = attribute::FIXED;
}
void layout_horizontal::on_update() {
  auto offset = 0;
  auto &current = get_attribute();
  for (auto &c : get_children()) {
    auto &attr = c->get_attribute();
    if (attr.position == attribute::RELATIVE) {
      auto rc = c->get_bound_rect();
      attr.offset.y = 0;
      attr.offset.x = offset;
      offset += rc.width;
      current.size.height += rc.height;
      if (current.size.width < rc.width) {
        current.size.width = rc.width;
      }
    }
  }
}