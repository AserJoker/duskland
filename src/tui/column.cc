#include "tui/column.hpp"
using namespace duskland::tui;
column::column() { set_selectable(true); }
void column::on_render(const core::auto_release<graphic> &g) {
  auto &rc = get_rect();
  auto offset = 0;
  for (auto &c : get_children()) {
    if (c->is_visible()) {
      auto crc = c->get_rect();
      c->set_rect({rc.x, rc.y + offset, crc.width, crc.height});
      offset += crc.height;
    }
  }
}
void column::on_update() {
  auto rc = get_rect();
  for (auto &c : get_children()) {
    auto &crc = c->get_rect();
    rc.height += crc.height;
    if (crc.width > rc.width) {
      rc.width = crc.width;
    }
  }
  set_rect(rc);
  if (get_parent()) {
    get_parent()->request_update();
  }
}
void column::on_active() {
  if (!get_active_widget()) {
    next_active();
  }
  widget::on_active();
}