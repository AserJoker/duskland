#include "tui/widget_text.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_text::widget_text(const std::string &name, const std::wstring &text)
    : widget(name), _text(text) {
  auto &rc = get_rect();
  rc.width = _text.length();
  rc.height = 1;
}
void widget_text::set_text(const std::wstring &value) {
  _text = value;
  auto &rc = get_rect();
  rc.width = _text.length();
  rc.height = 1;
}
const std::wstring &widget_text::get_text() { return _text; }
void widget_text::render(const core::auto_release<window> &win,
                         const util::position &pos) {
  win->write(pos.x, pos.y, _text.c_str(),
             is_active() ? WA_STANDOUT : WA_NORMAL);
}