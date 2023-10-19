#include "tui/widget_col.hpp"
using namespace duskland::tui;
using namespace duskland;
void widget_col::render(const core::auto_release<window> &win) {
  auto &children = get_children();
  int32_t y = 0;
  auto &crc = get_rect();
  for (auto c : children) {
    auto &rc = c->get_rect();
    c->set_rect({crc.x, crc.y + y, rc.width, rc.height});
    c->render(win);
    y += rc.height;
  }
}
widget_col::widget_col(const std::string &name) : widget_container(name) {}
void widget_col::on_children_change() {
  auto rc = get_rect();
  rc.width = 0;
  rc.height = 0;
  for (auto &w : get_children()) {
    auto &wrc = w->get_rect();
    if (wrc.width > rc.width) {
      rc.width = wrc.width;
    }
    rc.height += wrc.height;
  }
  set_rect(rc);
}