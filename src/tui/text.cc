#include "tui/text.hpp"
using namespace duskland::tui;
text::text(const std::wstring &text) { set_text(text); }
void text::set_text(const std::wstring &text) {
  auto &attr = get_attribute();
  _lines.clear();
  _lines.push_back(text);
  attr.size.height = 1;
  for (auto &c : text) {
    attr.size.width += wcwidth(c);
  }
  request_update();
}
std::wstring text::get_text() const {
  std::wstring result;
  for (auto &line : _lines) {
    result += line;
  }
  return result;
}
void text::on_render(core::auto_release<graphic> &g) {
  auto y = 0;
  for (auto &line : _lines) {
    g->draw(0, y, _lines[y]);
    y++;
  }
}
void text::on_update() {
  auto &attr = get_attribute();
  auto max_size = attr.max_size.width;
  auto text = get_text();
  if (_auto_wrap && get_parent() &&
      get_parent()->get_attribute().overflow == attribute::FIXED) {
    auto &attr = get_attribute();
    attr.size.height = 1;
    for (auto &c : text) {
      attr.size.width += wcwidth(c);
    }
    max_size = get_parent()->get_rect().width - attr.offset.x;
  }
  if (max_size) {
    if (attr.size.width > max_size) {
      _lines.clear();
      std::wstring line;
      auto width = 0;
      attr.size.width = max_size;
      attr.size.height = 0;
      auto index = 0;

      while (index < text.length()) {
        auto &c = text[index];
        auto cwidth = wcwidth(c);
        if (width + cwidth > max_size) {
          _lines.push_back(line);
          line.clear();
          width = 0;
          attr.size.height++;
        } else {
          line += c;
          width += cwidth;
          index++;
        }
      }
      if (!line.empty()) {
        _lines.push_back(line);
        attr.size.height++;
      }
    }
  }
}
void text::set_auto_wrap(const bool &auto_wrap) {
  _auto_wrap = auto_wrap;
  request_update();
}
const bool &text::get_auto_wrap() const { return _auto_wrap; }