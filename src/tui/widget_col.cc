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