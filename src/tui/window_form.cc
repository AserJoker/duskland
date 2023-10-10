#include "tui/window_form.hpp"
using namespace duskland::tui;
using namespace duskland;
window_form::window_form(const util::rect &rc, const std::string &name)
    : window(rc, name), _active_widget(nullptr) {}
std::vector<core::auto_release<widget_base>> &window_form::get_widgets() {
  return _widgets;
}
void window_form::on_update() {
  auto &rc = get_client_rect();
  auto [x, y, width, height] = rc;
  x = 0;
  y = 0;
  for (auto &widget : _widgets) {
    auto &wrc = widget->get_rect();
    wrc.x = x;
    wrc.y = y;
    wrc.width = rc.width;
    y += wrc.height;
    widget->render(this);
  }
  window::on_update();
}
void window_form::on_active() {
  window::on_active();
  if (_active_widget) {
    _active_widget->on_active();
  }
}
void window_form::on_dective() {
  window::on_dective();
  if (_active_widget) {
    _active_widget->on_dective();
  }
}
void window_form::on_command(int cmd) {
  if (_active_widget) {
    _active_widget->on_command(cmd);
  }
}
void window_form::set_active_widget(widget_base *widget) {
  _active_widget = widget;
}