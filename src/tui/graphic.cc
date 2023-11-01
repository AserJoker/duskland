#include "tui/graphic.hpp"
#include "util/injector.hpp"
using namespace duskland::tui;
graphic::graphic() : _need_update(false), _position({0, 0}) {
  initscr();
  start_color();
  raw();
  noecho();
  clear();
  curs_set(0);
  refresh();
}
graphic::~graphic() { endwin(); }
void graphic::draw(int32_t x, int32_t y, wchar_t ch) {
  cchar_t cc = {0, {ch, 0}};
  mvadd_wch(y, x, &cc);
  _need_update = true;
}
void graphic::draw(int32_t x, int32_t y, const std::wstring &str) {
  auto offset = 0;
  for (auto &c : str) {
    draw(x + offset, y, c);
    offset += wcwidth(c);
  }
}
void graphic::set_attr(chtype attr) { attrset(attr); }
bool graphic::present() {
  if (_need_update) {
    refresh();
    _need_update = false;
    return true;
  }
  return false;
}