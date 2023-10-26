#include "tui/window_widget.hpp"
using namespace duskland::tui;
using namespace duskland;
window_widget::window_widget(const std::string &name) : window(name) {}
void window_widget::on_initialize() { window::on_initialize(); };
void window_widget::on_active() {
  window::on_active();
  if (_root != nullptr) {
    _root->on_active();
  }
};
void window_widget::on_dective() {
  window::on_dective();
  if (_root != nullptr) {
    _root->on_dective();
  }
};
bool window_widget::on_command(const util::key &cmd) {
  if (_root != nullptr) {
    if (_root->on_command(this, cmd)) {
      return true;
    }
  }
  return window::on_command(cmd);
};
void window_widget::on_render() {
  window::on_render();
  draw_scroll();
  refresh();
  if (_root != nullptr) {
    _root->render(this);
  }
};
void window_widget::on_dispose() { window::on_dispose(); };
core::auto_release<widget> &window_widget::get_root() { return _root; }