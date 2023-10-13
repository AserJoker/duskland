#include "tui/widget_text.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_text::widget_text(const std::string &name, const std::wstring &text,
                         const int32_t &max_length)
    : widget(name), _max_length(max_length) {
  set_text(text);
}
void widget_text::set_text(const std::wstring &text) {
  _text.clear();
  if (_max_length) {
    auto &rc = get_rect();
    rc.width = _max_length;
    std::wstring item = text;
    while (item.length() > _max_length) {
      _text.push_back(item.substr(0, _max_length));
      item = item.substr(_max_length);
    }
    if (item.length()) {
      _text.push_back(item);
    }
    rc.height = _text.size();
  } else {
    _text.push_back(text);
    auto &rc = get_rect();
    auto size = 0;
    for (auto i = 0; i < text.length(); i++) {
      size += wcwidth(text[i]);
    }
    rc.width = size;
    rc.height = 1;
  }
}
const std::wstring widget_text::get_text() {
  std::wstring result;
  for (auto &item : _text) {
    result += item;
  }
  return result;
}
void widget_text::render(const core::auto_release<window> &win,
                         const util::position &pos) {
  auto y = 0;
  for (auto &str : _text) {
    win->write(pos.x, pos.y + y, str.c_str(),
               is_active() ? _config->attr("tui.text.focus")
                           : _config->attr("tui.text.normal"));
    y++;
  }
}