#include "tui/graphic.hpp"
#include <curses.h>
using namespace duskland::tui;
using namespace duskland;
graphic::graphic() : _need_update(false), _viewport({0, 0, 0, 0}) {}
graphic::~graphic() { endwin(); }
void graphic::initialize(const core::auto_release<util::color> &color) {
  initscr();
  start_color();
  raw();
  noecho();
  clear();
  curs_set(0);
  refresh();
  _colors = color;
}
void graphic::uninitialize() {}
void graphic::draw(int32_t x, int32_t y, wchar_t ch) {
  auto xx = x + _position.x;
  auto yy = y + _position.y;
  if (xx < _viewport.x || yy < _viewport.y) {
    return;
  }
  auto width = wcwidth(ch);
  if (_viewport.width && _viewport.height) {
    if (xx + width > _viewport.x + _viewport.width) {
      return;
    }
    if (yy + 1 > _viewport.y + _viewport.height) {
      return;
    }
  }
  cchar_t cc = {0, {ch, 0}};
  mvadd_wch(yy, xx, &cc);
  _need_update = true;
}
void graphic::draw_abstruct(int32_t x, int32_t y, wchar_t ch) {
  cchar_t cc = {0, {ch, 0}};
  auto width = wcwidth(ch);
  if (_viewport.width && _viewport.height) {
    if (x + width > _viewport.x + _viewport.width) {
      return;
    }
    if (y + 1 > _viewport.y + _viewport.height) {
      return;
    }
  }
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
void graphic::set_attr(const std::string &name) {
  attrset(_colors->query(name));
}
bool graphic::present() {
  if (_need_update) {
    refresh();
    _need_update = false;
    return true;
  }
  return false;
}
void graphic::set_viewport(const util::rect &rc) { _viewport = rc; }
const util::rect &graphic::get_viewport() const { return _viewport; }
void graphic::set_position(const util::point &pos) { _position = pos; }

const util::point &graphic::get_position() { return _position; }