#include "tui/widget_input.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_input::widget_input(const std::string &name, const uint32_t &length)
    : widget(name), _is_reading(false), _cursor(0), _max_length(length) {
  auto &rc = get_rect();
  set_rect({rc.x, rc.y, length, 1});
}
void widget_input::render(const core::auto_release<window> &win) {
  auto &rc = get_rect();
  auto text_attr = !is_active() ? _injector->attr("tui.input.normal")
                                : _injector->attr("tui.input.focus");
  std::wstring str = _value;
  str += std::wstring(_max_length - _value.length(), L'_');
  win->draw(rc.x, rc.y, str, text_attr);
  if (_is_reading) {
    auto offset = 0;
    for (auto i = 0; i < _cursor; i++) {
      offset += wcwidth(str[i]);
    }
    win->draw(rc.x + offset, rc.y, str[_cursor],
              _injector->attr("tui.input.cursor"));
  }
  win->refresh();
}
bool widget_input::on_command(const core::auto_release<window> &win,
                              const util::key &cmd) {
  if (cmd.name() == "<enter>") {
    if (!_is_reading) {
      _is_reading = true;
    } else {
      _is_reading = false;
      win->on_emit(this, "change");
    }
    win->render();
    return true;
  }
  if (cmd.name() == "<esc>") {
    if (_is_reading) {
      _is_reading = false;
      win->on_emit(this, "change");
      win->render();
      return true;
    }
    return false;
  }
  if (_is_reading) {
    if (cmd.name() == "<end>") {
      _cursor = _value.length();
      if (_cursor == _max_length) {
        _cursor = _max_length - 1;
      }
    } else if (cmd.name() == "<home>") {
      _cursor = 0;
    } else if (cmd.name() == "<tab>") {
      add_char(L' ', win);
    } else if (cmd.name() == "<backspace>") {
      if (_value.length() == 0) {
        beep();
      } else if (_cursor == 0) {
        _value.erase(_cursor, 1);
        auto c = _value[_cursor];
        if (wcwidth(c) > 1) {
          auto &rc = get_rect();
          set_rect({rc.x, rc.y, rc.width - (wcwidth(c) - 1), rc.height});
        }
      } else if (_cursor == _value.length() - 1) {
        _value.erase(_cursor, 1);
        auto c = _value[_cursor];
        if (wcwidth(c) > 1) {
          auto &rc = get_rect();
          set_rect({rc.x, rc.y, rc.width - (wcwidth(c) - 1), rc.height});
        }
      } else {
        _value.erase(_cursor - 1, 1);
        _cursor--;
        auto c = _value[_cursor - 1];
        if (wcwidth(c) > 1) {
          auto &rc = get_rect();
          set_rect({rc.x, rc.y, rc.width - (wcwidth(c) - 1), rc.height});
        }
      }
      win->on_emit(this, "input");
    } else if (cmd.name() == "<left>") {
      _cursor--;
      if (_cursor < 0) {
        _cursor = 0;
        beep();
      }
    } else if (cmd.name() == "<right>") {
      _cursor++;
      if (_cursor > _value.length()) {
        _cursor = _value.length();
        beep();
      }
      if (_cursor == _max_length) {
        _cursor = _max_length - 1;
        beep();
      }
    } else if (cmd.raw.size() == 1) {
      add_char(cmd.decode, win);
    }
    win->render();
    return true;
  }
  return widget::on_command(win, cmd);
}

void widget_input::add_char(const wchar_t &ch,
                            const core::auto_release<window> &win) {
  if (_value.length() < _max_length) {
    wchar_t s[2] = {(wchar_t)ch, 0};
    _value.insert(_cursor, &s[0]);
    _cursor++;
    if (_cursor == _max_length) {
      _cursor = _max_length - 1;
    }
    if (wcwidth(ch) > 1) {
      auto &rc = get_rect();
      set_rect({rc.x, rc.y, rc.width + (uint32_t)wcwidth(ch) - 1, rc.height});
    }
    win->on_emit(this, "input");
  } else {
    beep();
  }
}
const std::wstring &widget_input::get_value() const { return _value; }