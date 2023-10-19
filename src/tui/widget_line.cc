#include "tui/widget_line.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_line::widget_line(const std::string &name) : widget_container(name) {}
void widget_line::on_children_change() {
  auto rc = get_rect();
  rc.width = 0;
  rc.height = 0;
  for (auto &w : get_children()) {
    auto &wrc = w->get_rect();
    if (wrc.height > rc.height) {
      rc.height = wrc.height;
    }
    rc.width += wrc.width;
  }
  set_rect(rc);
}
void widget_line::render(const core::auto_release<window> &win) {
  auto &children = get_children();
  int32_t x = 0;
  auto &crc = get_rect();
  for (auto c : children) {
    auto &rc = c->get_rect();
    c->set_rect({crc.x + x, crc.y, rc.width, rc.height});
    c->render(win);
    x += rc.width;
  }
}