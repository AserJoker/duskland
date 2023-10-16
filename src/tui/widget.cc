#include "tui/widget.hpp"
#include "core/singleton.hpp"
#include "util/event.hpp"
using namespace duskland::tui;
using namespace duskland;
widget::widget(const std::string &name) : widget_base(name) {
  _injector = core::singleton<util::injector>::get();
  _tui = core::singleton<system_tui>::get();
}
void widget::render(const core::auto_release<window> &win,
                    const util::position &pos) {}

bool widget::on_command(wint_t cmd,
                        const core::auto_release<widget_base> &emitter) {
  if (cmd == _injector->keymap("key.select")) {
    if (emitter != nullptr) {
      emitter->on_command(EVENT_SELECT, this);
      return true;
    }
  }
  return widget_base::on_command(cmd, emitter);
}

void widget::on_active() {
  auto &rc = get_rect();
  auto win = _tui->get_active_window();
  if (win) {
    win->set_current_pos(rc.y, rc.x);
  }
  widget_base::on_active();
}