#include "tui/widget_text.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_text::widget_text(const std::string &name, const std::wstring &text)
    : widget(name), _text(text) {
  auto rc = get_rect();
  rc.height = 1;
  rc.width = 0;
  for (auto &c : text) {
    rc.width += wcwidth(c);
  }
  set_rect(rc);
}
void widget_text::render(const core::auto_release<window> &win) {
  auto &rc = get_rect();
  win->draw(rc.x, rc.y, _text,
            is_active() ? _injector->attr("tui.text.focus")
                        : _injector->attr("tui.text.normal"));
  win->refresh();
}