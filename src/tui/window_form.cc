#include "tui/window_form.hpp"
using namespace duskland::tui;
using namespace duskland;
window_form::window_form(const util::rect &rc, const std::string &name)
    : window(rc, name) {
  _form = new widget_form(name + ".form");
}
core::auto_release<widget_form> window_form::get_form() { return _form; }
void window_form::on_update() { _form->render(this, {0, 0}); }
void window_form::on_active() { _form->on_active(); }
void window_form::on_dective() { _form->on_dective(); }
bool window_form::on_command(int cmd,
                             const core::auto_release<widget_base> &emitter) {
  _form->on_command(cmd, this);
  return window::on_command(cmd, emitter);
}