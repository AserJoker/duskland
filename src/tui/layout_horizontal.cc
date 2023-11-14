#include "tui/layout_horizontal.hpp"
using namespace duskland::tui;
void layout_horizontal::on_update() {
  auto offset = 0;
  for (auto &c : get_children()) {
    auto &attr = c->get_attribute();
    if (attr.position == attribute::RELATIVE) {
      attr.offset.y = 0;
      attr.offset.x = offset;
      offset += c->get_bound_rect().width;
    }
  }
}