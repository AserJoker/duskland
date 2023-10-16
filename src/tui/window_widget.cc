#include "tui/window_widget.hpp"
using namespace duskland::tui;
using namespace duskland;

window_widget::window_widget(const util::rect &rc, const std::string &name)
    : window(rc, name) {}
core::auto_release<widget> &window_widget::get_root() { return _root; }
void window_widget::on_update() {
  if (_root != nullptr) {
    _root->render(this, {0, 0});
  }
}
void window_widget::on_active() {
  if (_root != nullptr && _root->get_select_index() != -1) {
    _root->on_active();
  }
}
void window_widget::on_dective() {
  if (_root != nullptr) {
    _root->on_dective();
  }
}
bool window_widget::on_command(wint_t cmd,
                               const core::auto_release<widget_base> &emitter) {
  if (_root != nullptr) {
    auto res = _root->on_command(cmd, this);
    if (!res) {
      if (cmd == _config->keymap("key.next")) {
        if (_root->get_select_index() != -1) {
          _root->on_active();
        }
      }
    }
    render();
  }
  return window::on_command(cmd, emitter);
}