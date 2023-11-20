﻿#include "tui/checkbox.hpp"
using namespace duskland::tui;
checkbox::checkbox(const std::wstring &name, const std::wstring &display_name)
    : component(name) {
  if (display_name.empty()) {
    _display_name = name;
  } else {
    _display_name = display_name;
  }
  set_value(false);
  auto &attr = get_attribute();
  attr.size.height = 1;
  attr.size.width = wcswidth(_display_name.c_str(), _display_name.length()) + 1;
  request_update();
}
void checkbox::on_render(core::auto_release<graphic> &g) {
  if (get_value()) {
    g->draw(0, 0, L'+');
  } else {
    g->draw(0, 0, L'-');
  }
  g->draw(1, 0, _display_name);
}
bool checkbox::on_input(const util::key &key) {
  if (key.name == "<enter>") {
    set_value(!get_value());
    return true;
  }
  return component::on_input(key);
}