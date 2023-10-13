#include "tui/widget_input.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_input::widget_input(const std::string &name, const uint32_t &max_length)
    : widget(name), _max_length(max_length), _mode(0) {
  get_rect().width = max_length + 2;
  get_rect().height = 1;
}
const std::wstring &widget_input::get_input() const { return _input; }
bool widget_input::on_command(int cmd,
                              const core::auto_release<widget_base> &emitter) {
  auto win = (window *)emitter.get();
  auto &rc = get_rect();
  if (_mode == 0 && cmd == '\n') {
    _mode = 1;
    win->update();
    return true;
  }
  if (_mode == 1) {
    if (cmd == '\n' || cmd == 0x1b) {
      _mode = 0;
      win->update();
    } else if (cmd == KEY_BACKSPACE) {
      if (_input.empty()) {
        beep();
      } else {
        _input = _input.substr(0, _input.length() - 1);
      }
    } else if ((cmd >= 0x20 && cmd <= KEY_MIN) || cmd >= KEY_MAX) {
      if (_input.length() == _max_length) {
        beep();
      } else {
        _input += cmd;
      }
    }
    auto offset = 0;
    for (auto &c : _input) {
      offset += wcwidth(c);
    }
    rc.width = offset + _max_length - _input.length() + 2;
    win->update();
    return true;
  }
  return widget::on_command(cmd, emitter);
}
void widget_input::render(const core::auto_release<window> &win,
                          const util::position &pos) {
  std::wstring renderstr = _input;
  for (auto i = _input.length(); i != _max_length; i++) {
    renderstr += L'_';
  }
  win->write(pos.x, pos.y, renderstr.c_str(),
             is_active() ? _config->attr("tui.input.focus")
                         : _config->attr("tui.input.normal"));
  if (_mode) {
    auto offset = 0;
    for (auto &c : _input) {
      offset += wcwidth(c);
    }
    if (_input.length() < _max_length) {
      win->write(pos.x + offset, pos.y, L'_',
                 _config->attr("tui.input.cursor"));
    } else {
      auto chr = _input[_input.length() - 1];
      offset -= wcwidth(chr);
      win->write(pos.x + offset, pos.y, chr, _config->attr("tui.input.cursor"));
    }
  }
}