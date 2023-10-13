#include "tui/widget_line.hpp"
using namespace duskland::tui;
using namespace duskland;
void widget_line::render(const core::auto_release<window> &win,
                         const util::position &pos) {
  auto x = pos.x;
  for (auto &w : get_widgets()) {
    w->render(win, {x, pos.y});
    x += w->get_rect().width;
  }
}
widget_line::widget_line(const std::string &name) : widget_container(name) {}
util::size widget_line::get_bound_size() {
  auto &client_rc = get_rect();
  util::size size = {0, 0};
  for (auto &w : get_widgets()) {
    auto &rc = w->get_rect();
    rc.y = client_rc.y;
    rc.x = client_rc.x + size.width;
    size.width += rc.width;
    size.height = std::max(size.height, rc.height);
  }
  client_rc.width = size.width;
  client_rc.height = size.height;
  return size;
}