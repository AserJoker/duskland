#include "tui/window.hpp"
#include "core/singleton.hpp"
#include "tui/system_tui.hpp"
using namespace duskland::tui;
using namespace duskland;
window::window(const util::rect &rc, const std::string &name)
    : _border_style({0, 0, 0, 0, 0, 0, 0, 0}),
      _border({false, false, false, false}), _is_active(false), _name(name) {
  _tui = core::singleton<system_tui>::get();
  _rect = rc;
  _win = newwin(_rect.height, _rect.width, _rect.y, _rect.x);
  box(_win, 0, 0);
  set_border({false, false, false, false});
  set_border_color(0);
  _tui->add_window(this);
}
window::~window() {
  set_border({false, false, false, false});
  _tui->remove_window(this);
  delwin(_win);
  _win = nullptr;
}
void window::refresh() {
  util::border border = _border;
  set_border({false, false, false, false});
  delwin(_win);
  _win = newwin(_rect.height, _rect.width, _rect.y, _rect.x);
  box(_win, 0, 0);
  set_border(border);
}
void window::set_border(const util::border &border) {
  if (!_win) {
    return;
  }
  _border = border;
  update();
}
const util::border &window::get_border() const { return _border; }

void window::draw_border() {
  util::border_style b = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
  if (_border.left) {
    b.ls = _border_style.ls;
  }
  if (_border.right) {
    b.rs = _border_style.rs;
  }
  if (_border.top) {
    b.ts = _border_style.ts;
  }
  if (_border.bottom) {
    b.bs = _border_style.bs;
  }
  if (_border.left && _border.top) {
    b.tl = _border_style.tl;
  }
  if (_border.top && _border.right) {
    b.tr = _border_style.tr;
  }
  if (_border.bottom && _border.left) {
    b.bl = _border_style.bl;
  }
  if (_border.bottom && _border.right) {
    b.br = _border_style.br;
  }
  wattron(_win, COLOR_PAIR(_border_color));
  wborder(_win, b.ls, b.rs, b.ts, b.bs, b.tl, b.tr, b.bl, b.br);
  wattroff(_win, COLOR_PAIR(_border_color));
}
void window::update() {
  clear();
  box(_win, 0, 0);
  draw_border();
  on_update();
  wrefresh(_win);
}

const util::rect &window::get_rect() const { return _rect; }

void window::fix_rect() {
  if (_rect.x < 0) {
    _rect.x = 0;
  }
  if (_rect.y < 0) {
    _rect.y = 0;
  }
  if (_rect.width < 10) {
    _rect.width = 10;
  }
  if (_rect.height < 3) {
    _rect.height = 3;
  }
}

void window::resize(int32_t dw, int32_t dh) {
  if (dw != 0 || dh != 0) {
    if (_rect.x + _rect.width + dw <= getmaxx(stdscr)) {
      _rect.width += dw;
    }
    if (_rect.y + _rect.height + dh <= getmaxy(stdscr)) {
      _rect.height += dh;
    }
    fix_rect();
    refresh();
  }
}
void window::move(int32_t dx, int32_t dy) {
  if (dx != 0 || dy != 0) {
    if (_rect.x + _rect.width + dx <= getmaxx(stdscr)) {
      _rect.x += dx;
    }
    if (_rect.y + _rect.height + dy <= getmaxy(stdscr)) {
      _rect.y += dy;
    }
    fix_rect();
    refresh();
  }
}
void window::set_rect(const util::rect &rc) {
  if (rc.x == _rect.x && rc.y == _rect.y && rc.width == _rect.width &&
      rc.height == _rect.height) {
    return;
  }
  _rect = rc;
  fix_rect();
  refresh();
}
void window::set_border_color(int index) {
  _border_color = index;
  wattron(_win, COLOR_PAIR(index));
  box(_win, 0, 0);
  set_border(_border);
  wattroff(_win, COLOR_PAIR(index));
  update();
}
void window::on_active() {
  _is_active = true;
  update();
}
void window::on_dective() {
  _is_active = false;
  update();
}
void window::on_command(int cmd) {}
void window::on_update() {
  if (_is_active) {
    wattron(_win, COLOR_PAIR(2));
    mvwprintw(_win, 1, 2, _name.c_str());
    wattroff(_win, COLOR_PAIR(2));
  } else {
    wattron(_win, COLOR_PAIR(2) | WA_DIM);
    mvwprintw(_win, 1, 2, _name.c_str());
    wattroff(_win, COLOR_PAIR(2) | WA_DIM);
  }
}
void window::active() { _tui->set_active_window(this); }
bool window::is_active() { return _is_active; }
void window::clear() {
  for (int x = 1; x < _rect.width - 1; x++) {
    for (int y = 1; y < _rect.height - 1; y++) {
      mvwaddch(_win, x, y, ' ');
    }
  }
}
const util::rect window::get_client_rect() const {
  return {1, 1, _rect.width - 2, _rect.height - 2};
}
const std::string &window::get_name() const { return _name; }
void window::set_name(const std::string &name) {
  _name = name;
  update();
}