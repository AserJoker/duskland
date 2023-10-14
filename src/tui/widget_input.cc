#include "tui/widget_input.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_input::widget_input(const std::string &name, const uint32_t &max_length)
    : widget(name), _max_length(max_length), _mode(0) {
  get_rect().width = max_length + 2;
  get_rect().height = 1;
}
const std::wstring &widget_input::get_input() const { return _input; }
bool widget_input::on_command(wint_t cmd,
                              const core::auto_release<widget_base> &emitter) {
  auto win = (window *)emitter.get();
  auto &rc = get_rect();
  if (cmd == '\n') {
    _mode = !_mode;
  }
  return widget::on_command(cmd, emitter);
}
void widget_input::render(const core::auto_release<window> &win,
                          const util::position &pos) {
  std::wstring renderstr = _input;
  for (auto i = _input.length(); i != _max_length; i++) {
    renderstr += L'_';
  }
  win->write(pos.x + 1, pos.y, renderstr.c_str(),
             is_active() ? _config->attr("tui.input.focus")
                         : _config->attr("tui.input.normal"));
  if (_mode) {
    auto offset = 0;
    for (auto &c : _input) {
      offset += wcwidth(c);
    }
    if (_input.length() < _max_length) {
      win->write(pos.x + 1 + offset, pos.y, L'_',
                 _config->attr("tui.input.cursor"));
    } else {
      auto chr = _input[_input.length() - 1];
      offset -= wcwidth(chr);
      win->write(pos.x + 1 + offset, pos.y, chr,
                 _config->attr("tui.input.cursor"));
    }
  }
}