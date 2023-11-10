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
  if (attr.max_size.width) {
    if (attr.size.width > attr.max_size.width) {
      auto text = get_text();
      _lines.clear();
      std::wstring line;
      auto width = 0;
      attr.size.width = attr.max_size.width;
      attr.size.height = 0;
      auto index = 0;

      while (index < text.length()) {
        auto &c = text[index];
        auto cwidth = wcwidth(c);
        if (width + cwidth > attr.max_size.width) {
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