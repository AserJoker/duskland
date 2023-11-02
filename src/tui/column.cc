﻿#include "tui/column.hpp"
using namespace duskland::tui;
column::column() { set_selectable(true); }
void column::on_update() {
  auto rc = get_rect();
  auto &content_rc = get_content_rect();
  rc.height = 0;
  rc.width = 0;
  auto offset = 0;
  for (auto &c : get_children()) {
    if (c->is_visible()) {
      auto &crc = c->get_rect();
      rc.height += crc.height;
      if (crc.width > rc.width) {
        rc.width = crc.width;
      }
      c->set_rect({content_rc.x, content_rc.y + offset, crc.width, crc.height});
      offset += crc.height;
    }
  }
  set_content_rect(rc);
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