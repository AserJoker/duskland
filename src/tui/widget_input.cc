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
  if (cmd.decode == '\n') {
    if (!_is_reading) {
      _injector->feature("feature.active_lock", true);
      _injector->feature("feature.text_input", true);
      keypad(stdscr, FALSE);
      _is_reading = true;
    } else {
      keypad(stdscr, TRUE);
      _is_reading = false;
      _injector->feature("feature.text_input", false);
      _injector->feature("feature.active_lock", false);
    }
    win->render();
    return true;
  }
  if (_is_reading) {
    if (cmd.decode == KEY_END) {
      _cursor = _value.length();
      if (_cursor == _max_length) {
        _cursor = _max_length - 1;
      }
    } else if (cmd.decode == KEY_HOME) {
      _cursor = 0;
    } else if (cmd.raw[0] == '\t') {
      add_char(L' ');
    } else if (cmd.raw[0] == 0x7f) {
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
    } else if (cmd.decode == KEY_LEFT) {
      _cursor--;
      if (_cursor < 0) {
        _cursor = 0;
        beep();
      }
    } else if (cmd.decode == KEY_RIGHT) {
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
      add_char(cmd.decode);
    }
    win->render();
    return true;
  }
  return widget::on_command(win, cmd);
}

void widget_input::add_char(const wchar_t &ch) {
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
  } else {
    beep();
  }
}