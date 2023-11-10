#include "tui/layout_vertical.hpp"
using namespace duskland::tui;
void layout_vertical::on_update() {
  auto offset = 0;
  for (auto &c : get_children()) {
    auto &attr = c->get_attribute();
    if (attr.position == attribute::RELATIVE) {
      attr.offset.y = offset;
      attr.offset.x = 0;
      offset += c->get_bound_rect().height;
    }
  }
}