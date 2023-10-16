#include "tui/widget_input.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_input::widget_input(const std::string &name, const uint32_t &max_length)
    : widget(name), _max_length(max_length), _mode(0), _cursor(0) {
  get_rect().width = max_length + 2;
  get_rect().height = 1;
}
const std::wstring &widget_input::get_input() const { return _input; }
void widget_input::on_input(const wchar_t &ch) {
  if (_input.length() < _max_length) {
    if (_cursor == _input.length()) {
      _input += ch;
      if (_cursor < _max_length - 1) {
        _cursor++;
      }
    } else {
      wchar_t c[] = {(wchar_t)ch, 0};
      _input.insert(_cursor, c);
    }
  } else {
    beep();
  }
}
void widget_input::on_backspace() {
  if (_input.empty()) {
    beep();
  } else if (_cursor == _input.length() - 1) {
    _input = _input.substr(0, _input.length() - 1);
    if (_cursor != _max_length - 1) {
      _cursor--;
    }
  } else {
    if (_cursor > 0) {
      _input.erase(_cursor - 1, 1);
      _cursor--;
    } else {
      _input.erase(0, 1);
    }
  }
}
bool widget_input::on_command(wint_t cmd,
                              const core::auto_release<widget_base> &emitter) {
  auto win = (window *)emitter.get();
  auto &rc = get_rect();
  if (cmd == '\n' && !_mode) {
    _mode = true;
    win->render();
    for (;;) {
      std::vector<wint_t> codes;
      for (;;) {
        wint_t ch = 0;
        if (wget_wch(stdscr, &ch) != ERR) {
          codes.push_back(ch);
        } else {
          break;
        }
      }
      if (codes.size()) {
        auto cmd = decode_input(codes);
        if (codes[0] == 0x1b) {
          if (cmd == 0x1b) {
            _mode = false;
            win->render();
            break;
          } else if (cmd == KEY_LEFT) {
            if (_cursor > 0) {
              --_cursor;
            } else {
              beep();
            }
          } else if (cmd == KEY_RIGHT) {
            if (_cursor < _input.size()) {
              _cursor++;
            } else {
              beep();
            }
          } else if (cmd == KEY_HOME) {
            _cursor = 0;
          } else if (cmd == KEY_END) {
            _cursor = _input.size();
            if (_cursor == _max_length) {
              _cursor = _max_length - 1;
            }
          }
          win->render();
        } else {
          for (auto &ch : codes) {
            if (ch == '\n') {
              _mode = false;
              break;
            } else if (cmd == KEY_BACKSPACE || cmd == KEY_DC) {
              on_backspace();
            } else if (ch == '\t') {
              on_input(L' ');
            } else {
              on_input(ch);
            }
            rc.width = 0;
            for (auto &c : _input) {
              rc.width += wcwidth(c);
            }
            rc.width += wcwidth('_') * _max_length - _input.length() + 2;
            win->render();
          }
          if (!_mode) {
            break;
          }
        }
      }
    }
    win->render();
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
    for (auto i = 0; i < _cursor; i++) {
      offset += wcwidth(renderstr[i]);
    }
    win->write(pos.x + 1 + offset, pos.y, renderstr[_cursor],
               _config->attr("tui.input.cursor"));
  }
}
wint_t widget_input::decode_input(const std::vector<wint_t> &codes) {
  if (codes.size() == 1) {
    return codes[0];
  } else if (codes.size() == 3) {
    if (codes[1] == 0x4f && codes[2] == 0x44) {
      return KEY_LEFT;
    }
    if (codes[1] == 0x4f && codes[2] == 0x43) {
      return KEY_RIGHT;
    }
    if (codes[1] == 0x4f && codes[2] == 0x48) {
      return KEY_HOME;
    }
    if (codes[1] == 0x4f && codes[2] == 0x46) {
      return KEY_END;
    }
  } else if (codes.size() == 4) {
    if (codes[1] == 0x5b && codes[2] == 0x33 && codes[3] == 0x7e) {
      return KEY_DC;
    }
  }
  return ERR;
}