#include "../include/text.hpp"
#include "core/include/auto_release.hpp"
#include "util/include/xchar.hpp"
#include <string>
using namespace duskland;
using namespace duskland::tui;
text::text(const std::wstring &text) { set_text(text); }
void text::set_text(const std::wstring &text) {
  _text = text;
  auto rc = get_size();
  rc.height = 1;
  rc.width = util::wcswidth(_text.c_str(), _text.length());
}
const std::wstring &text::get_text() const { return _text; }
void text::render(core::auto_release<graphic> &g) { g->draw(0, 0, _text); }