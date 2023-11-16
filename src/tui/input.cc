﻿#include "tui/input.hpp"
using namespace duskland::tui;
input::input(int32_t max_length)
    : _is_input(false), _cursor(0), _max_length(max_length) {
  get_attribute().size.width = max_length;
  request_update();
}
bool input::on_input(const util::key &key) {
  if (_is_input) {
    if (key.name == "<enter>" || key.name == "<esc>") {
      _is_input = false;
      request_update();
    } else if (key.name == "<backspace>" || key.name == "<delete>") {
      on_backspace();
    } else if (key.name == "<left>") {
      set_cursor(_cursor - 1);
    } else if (key.name == "<right>") {
      set_cursor(_cursor + 1);
    } else if (key.name == "<home>") {
      set_cursor(0);
    } else if (key.name == "<end>") {
      set_cursor(_max_length);
    } else if (!key.control) {
      on_input(key.raw[0]);
    }
    return true;
  } else {
    if (key.name == "<enter>") {
      _is_input = true;
      request_update();
      return true;
    }
  }
  return false;
}
void input::on_update() {
  widget::on_update();
  auto parent = get_parent();
  if (parent) {
    parent->request_update();
  }
}
void input::on_render(core::auto_release<graphic> &g) {
  auto &attr = get_attribute();
  std::wstring str = _value;
  str += std::wstring(_max_length - _value.size(), L'_');
  g->draw(0, 0, str);
  if (_is_input) {
    auto offset = 0;
    for (auto index = 0; index < str.size(); index++) {
      if (index == _cursor) {
        break;
      }
      offset += wcwidth(str[index]);
    }
    g->set_attr("cursor");
    g->draw(offset, 0, str[_cursor]);
  }
}

void input::on_input(wchar_t ch) {
  if (_value.length() == _max_length) {
    return;
  }
  wchar_t s[] = {ch, 0};
  _value.insert(_cursor, s);
  auto &attr = get_attribute();
  attr.size.width = 0;
  for (auto &c : _value) {
    attr.size.width += wcwidth(c);
  }
  attr.size.width += _max_length - _value.length();
  _cursor++;
  if (_cursor == _max_length) {
    _cursor = _max_length - 1;
  }
  request_update();
}
void input::on_backspace() {
  if (_cursor == _value.length() - 1) {
    _value.pop_back();
  } else if (_cursor < _value.length() - 1) {
    if (_cursor > 0) {
      _value.erase(_cursor - 1, 1);
      set_cursor(_cursor - 1);
    } else {
      _value.erase(0, 1);
    }
  } else {
    _value.pop_back();
    set_cursor(_cursor - 1);
  }
  auto &attr = get_attribute();
  attr.size.width = 0;
  for (auto &c : _value) {
    attr.size.width += wcwidth(c);
  }
  attr.size.width += _max_length - _value.length();
  request_update();
}
void input::set_cursor(int32_t cursor) {
  _cursor = cursor;
  if (_cursor < 0) {
    _cursor = 0;
  }
  if (_cursor > _value.length()) {
    _cursor = _value.length();
  }
  if (_cursor > _max_length - 1) {
    _cursor = _max_length - 1;
  }
  request_update();
}