#include "tui/widget_col.hpp"
using namespace duskland::tui;
using namespace duskland;
void widget_col::render(const core::auto_release<window> &win,
                        const util::position &pos) {
  auto y = pos.y;
  for (auto &w : get_widgets()) {
    w->render(win, {pos.x, y});
    y += w->get_rect().height;
  }
}
widget_col::widget_col(const std::string &name) : widget_container(name) {}
util::size widget_col::get_bound_size() {
  auto &custom_rc = get_rect();
  util::size size = {0, 0};
  for (auto &w : get_widgets()) {
    auto &rc = w->get_rect();
    rc.x = custom_rc.x;
    rc.y = custom_rc.y + size.height;
    size.height += rc.height;
    size.width = std::max(size.width, rc.width);
  }
  custom_rc.width = size.width;
  custom_rc.height = size.height;
  return size;
}