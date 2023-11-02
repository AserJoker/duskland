#include "tui/line.hpp"
using namespace duskland::tui;
line::line() { set_selectable(true); }
void line::on_update() {
  auto rc = get_rect();
  auto &content_rc = get_content_rect();
  rc.height = 0;
  rc.width = 0;
  auto offset = 0;
  for (auto &c : get_children()) {
    if (c->is_visible()) {
      auto &crc = c->get_rect();
      rc.width += crc.width;
      if (crc.height > rc.height) {
        rc.height = crc.height;
      }
      c->set_rect({offset, 0, crc.width, crc.height});
      offset += crc.width;
    }
  }
  set_content_rect(rc);
  if (get_parent()) {
    get_parent()->request_update();
  }
}