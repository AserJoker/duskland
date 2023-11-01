#include "tui/text.hpp"
using namespace duskland::tui;
text::text(const std::wstring &value) {
  set_selectable(true);
  set_text(value);
}
void text::set_text(const std::wstring &value) {
  _text = value;
  auto rc = get_rect();
  rc.width = 0;
  for (auto &c : _text) {
    rc.width += wcwidth(c);
  }
  rc.height = 1;
  set_rect(rc);
}
void text::on_render(const core::auto_release<graphic> &g) {
  auto &rc = get_rect();
  g->draw(rc.x, rc.y, _text);
}