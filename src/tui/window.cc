#include "tui/window.hpp"
#include "core/singleton.hpp"
#include "tui/system_tui.hpp"
#include "util/attriabute.hpp"
#include <fmt/format.h>
using namespace duskland::tui;
using namespace duskland;
window::window(const util::rect &rc, const std::string &name)
    : _border_style(BORDER_DEFAULT), _border({false, false, false, false}),
      widget(name), _current_line(0), _current_column(0) {
  _tui = core::singleton<system_tui>::get();
  _attribute = core::singleton<util::attribute>::get();
  widget::get_rect() = rc;
  _win = newwin(rc.height, rc.width, rc.y, rc.x);
  box(_win, 0, 0);
  set_border({false, false, false, false});
  _tui->add_window(this);
  _content_rect = {0, 0, rc.width, rc.height - 1};
}
window::~window() {
  set_border({false, false, false, false});
  _tui->remove_window(this);
  delwin(_win);
  _win = nullptr;
}
void window::refresh() {
  util::border border = _border;
  auto rc = get_rect();
  set_border({false, false, false, false});
  delwin(_win);
  _win = newwin(rc.height, rc.width, rc.y, rc.x);
  box(_win, 0, 0);
  set_border(border);
  fix_content_rect();
  update();
}
void window::set_border(const util::border &border) {
  if (!_win) {
    return;
  }
  _border = border;
}
const util::border &window::get_border() const { return _border; }
void window::set_border_style(const util::border_style &style) {
  _border_style = style;
}
void window::draw_border() {
  util::border_style b = {L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' '};
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
  cchar_t ls = {0, {b.ls, 0}};
  cchar_t rs = {0, {b.rs, 0}};
  cchar_t ts = {0, {b.ts, 0}};
  cchar_t bs = {0, {b.bs, 0}};
  cchar_t tl = {0, {b.tl, 0}};
  cchar_t tr = {0, {b.tr, 0}};
  cchar_t bl = {0, {b.bl, 0}};
  cchar_t br = {0, {b.br, 0}};
  wattron(_win, _attribute->attr("tui.border.attribute"));
  wborder_set(_win, &ls, &rs, &ts, &bs, &tl, &tr, &bl, &br);
  wattroff(_win, _attribute->attr("tui.border.attribute"));
  wrefresh(_win);
}
void window::update() {
  clear();
  draw_border();
  draw_scroll();
  on_update();
}

void window::fix_rect() {
  auto &_rect = widget::get_rect();
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
    auto &_rect = widget::get_rect();
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
    auto &_rect = widget::get_rect();
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
  auto &_rect = widget::get_rect();
  if (rc.x == _rect.x && rc.y == _rect.y && rc.width == _rect.width &&
      rc.height == _rect.height) {
    return;
  }
  _rect = rc;
  fix_rect();
  refresh();
}
void window::on_active() { widget::on_active(); }
void window::on_dective() { widget::on_dective(); }
void window::on_command(int cmd) {}
void window::on_update() {}
void window::active() { _tui->set_active_window(this); }
void window::clear() {
  auto &_rect = get_rect();
  for (int x = 0; x < _rect.width; x++) {
    for (int y = 0; y < _rect.height; y++) {
      mvwaddch(_win, y, x, ' ');
    }
  }
  wrefresh(_win);
}

void window::write(const uint32_t &x, const uint32_t &y, const wchar_t &ch,
                   const uint32_t &attr) {
  auto &rc = get_rect();
  uint32_t _x = x;
  uint32_t _y = y;
  if (_border.left) {
    _x++;
  }
  if (_border.top) {
    _y++;
  }
  if (_x >= _content_rect.width) {
    _content_rect.width = _x + 1;
    fix_content_rect();
  }
  auto xx = _x + _content_rect.x;
  auto yy = _y + _content_rect.y;
  if (_border.left && xx < 1) {
    return;
  } else if (_border.right && xx > rc.width - 2) {
    return;
  } else if (xx < 0 || xx > rc.width - 1) {
    return;
  }
  if (_border.top && yy < 1) {
    return;
  }
  if (yy < 0 || yy > rc.height - 2) {
    return;
  }
  cchar_t cch = {0, {ch, 0}};
  wattron(_win, attr);
  mvwadd_wch(_win, yy, xx, &cch);
  wattroff(_win, attr);
  wrefresh(_win);
}
void window::write(const uint32_t &x, const uint32_t &y, const wchar_t *str,
                   const uint32_t &attr) {
  for (auto index = 0; str[index] != 0; index++) {
    write(x + index, y, str[index], attr);
  }
}
void window::draw_scroll() {
  auto &rc = get_rect();
  if (_content_rect.width == rc.width &&
      _content_rect.height == rc.height - 1) {
    return;
  }
  wattron(_win, _attribute->attr("tui.border.attribute"));

  mvwprintw(_win, rc.height - 1, 1, "line: %d/%d column: %d/%d",
            -_content_rect.x + rc.width, _content_rect.width,
            -_content_rect.y + (rc.height - 1), _content_rect.height);
  wattroff(_win, _attribute->attr("tui.border.attribute"));
  wrefresh(_win);
}

void window::set_content_rect(const util::rect &content_rc) {
  _content_rect = content_rc;
  fix_content_rect();
}

const util::rect &window::get_content_rect() const { return _content_rect; }
void window::move_content(const int32_t &dx, const int32_t &dy) {
  auto &rc = get_rect();
  _content_rect.x += dx;
  _content_rect.y += dy;
  fix_content_rect();
}
void window::set_current_pos(const int32_t &line, const int32_t &column) {
  auto &rc = get_rect();
  _current_line = line;
  _current_column = column;
  if (_current_column < 0) {
    _current_column = 0;
  }
  if (_current_column >= _content_rect.width) {
    _current_column = _content_rect.width - 1;
  }
  if (_current_line < 0) {
    _current_line = 0;
  }
  if (_current_line >= _content_rect.height) {
    _current_line = _content_rect.height - 1;
  }
  _content_rect.y = (rc.height - 1) / 2 - _current_line;
  _content_rect.x = (rc.width) / 2 - _current_column;
  fix_content_rect();
}
void window::fix_content_rect() {
  auto &rc = get_rect();
  if (_content_rect.height < rc.height - 1) {
    _content_rect.height = rc.height - 1;
  }
  if (_content_rect.width < rc.width) {
    _content_rect.width = rc.width;
  }
  if (!_border.bottom) {
    if (_content_rect.y + _content_rect.height < rc.height - 1) {
      _content_rect.y = (rc.height - 1) - (_content_rect.height);
    }
  } else {
    if (_content_rect.y + _content_rect.height < rc.height - 2) {
      _content_rect.y = (rc.height - 2) - (_content_rect.height);
    }
  }
  if (_content_rect.x + _content_rect.width < rc.width) {
    _content_rect.x = (rc.width) - (_content_rect.width);
  }
  if (_content_rect.x > 0) {
    _content_rect.x = 0;
  }
  if (_content_rect.y > 0) {
    _content_rect.y = 0;
  }
}