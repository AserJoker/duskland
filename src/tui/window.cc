#include "tui/window.hpp"
#include "core/singleton.hpp"
#include "tui/system_tui.hpp"
#include "util/config.hpp"
#include <fmt/format.h>
using namespace duskland::tui;
using namespace duskland;
window::window(const util::rect &rc, const std::string &name)
    : _border({false, false, false, false}), widget_base(name),
      _current_line(0), _current_column(0) {
  _tui = core::singleton<system_tui>::get();
  _config = core::singleton<util::config>::get();
  auto &_rc = get_rect();
  get_rect() = rc;
  fix_rect();
  _win = newwin(rc.height, rc.width, rc.y, rc.x);
  box(_win, 0, 0);
  set_border({true, true, true, true});
  _tui->add_window(this);
  _content_rect = {0, 0, _rc.width, _rc.height - 1};
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
  clear();
  update();
}
void window::set_border(const util::border &border) {
  if (!_win) {
    return;
  }
  _border = border;
}
const util::border &window::get_border() const { return _border; }
void window::draw_border() {
  cchar_t ls = {0, {L' ', 0}};
  cchar_t rs = {0, {L' ', 0}};
  cchar_t ts = {0, {L' ', 0}};
  cchar_t bs = {0, {L' ', 0}};
  cchar_t tl = {0, {L' ', 0}};
  cchar_t tr = {0, {L' ', 0}};
  cchar_t bl = {0, {L' ', 0}};
  cchar_t br = {0, {L' ', 0}};
  if (_border.left) {
    ls.chars[0] = _config->style("style.border.ls");
  }
  if (_border.right) {
    rs.chars[0] = _config->style("style.border.rs");
  }
  if (_border.top) {
    ts.chars[0] = _config->style("style.border.ts");
  }
  if (_border.bottom) {
    bs.chars[0] = _config->style("style.border.bs");
  }
  if (_border.left && _border.top) {
    tl.chars[0] = _config->style("style.border.tl");
  }
  if (_border.top && _border.right) {
    tr.chars[0] = _config->style("style.border.tr");
  }
  if (_border.bottom && _border.left) {
    bl.chars[0] = _config->style("style.border.bl");
  }
  if (_border.bottom && _border.right) {
    br.chars[0] = _config->style("style.border.br");
  }
  wattron(_win, _config->attr("tui.border.normal"));
  wborder_set(_win, &ls, &rs, &ts, &bs, &tl, &tr, &bl, &br);
  wattroff(_win, _config->attr("tui.border.normal"));
  wrefresh(_win);
}
void window::update() {
  draw_border();
  draw_scroll();
  on_update();
}

void window::fix_rect() {
  auto &_rect = get_rect();
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
  fix_content_rect();
}

void window::resize(int32_t dw, int32_t dh) {
  if (dw != 0 || dh != 0) {
    auto &_rect = get_rect();
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
    auto &_rect = get_rect();
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
  auto &_rect = get_rect();
  if (rc.x == _rect.x && rc.y == _rect.y && rc.width == _rect.width &&
      rc.height == _rect.height) {
    return;
  }
  _rect = rc;
  fix_rect();
  refresh();
}
void window::on_active() { widget_base::on_active(); }
void window::on_dective() { widget_base::on_dective(); }
bool window::on_command(wint_t cmd,
                        const core::auto_release<widget_base> &emitter) {
  return widget_base::on_command(cmd, emitter);
}
void window::on_update() {}
void window::active() { _tui->set_active_window(this); }
void window::clear() {
  wattron(_win, COLOR_PAIR(COLOR_PAIR_INDEX(COLOR_WHITE, COLOR_BLACK)));
  auto &_rect = get_rect();
  for (int x = 0; x < _rect.width; x++) {
    for (int y = 0; y < _rect.height; y++) {
      mvwaddch(_win, y, x, ' ');
    }
  }
  wattroff(_win, COLOR_PAIR(COLOR_PAIR_INDEX(COLOR_WHITE, COLOR_BLACK)));
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
void window::write(const uint32_t &x, const uint32_t &y, const char &ch,
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
  wattron(_win, attr);
  mvwaddch(_win, yy, xx, ch);
  wattroff(_win, attr);
  wrefresh(_win);
}
void window::write(const uint32_t &x, const uint32_t &y, const wchar_t *str,
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
  cchar_t *cc = new cchar_t[wcslen(str) + 1];
  for (auto i = 0; i < wcslen(str); i++) {
    cc[i].attr = 0;
    cc[i].chars[0] = str[i];
    cc[i].chars[1] = 0;
  }
  cc[wcslen(str)].chars[0] = 0;
  wattron(_win, attr);
  mvwadd_wchstr(_win, yy, xx, cc);
  wattroff(_win, attr);
  wrefresh(_win);
  delete[] cc;
}
void window::write(const uint32_t &x, const uint32_t &y, const char *str,
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

  wattron(_win, attr);
  mvwaddstr(_win, yy, xx, str);
  wattroff(_win, attr);
  wrefresh(_win);
}
void window::draw_scroll() {
  auto &rc = get_rect();
  if (_content_rect.width == rc.width &&
      _content_rect.height == rc.height - 1) {
    return;
  }
  wattron(_win, _config->attr("tui.border.normal"));

  mvwprintw(_win, rc.height - 1, 1, "line: %d/%d column: %d/%d",
            -_content_rect.x + rc.width, _content_rect.width,
            -_content_rect.y + (rc.height - 1), _content_rect.height);
  wattroff(_win, _config->attr("tui.border.normal"));
  wrefresh(_win);
}

void window::set_content_rect(const util::rect &content_rc) {
  if (_content_rect.x != content_rc.x || _content_rect.y != content_rc.y ||
      _content_rect.width != content_rc.width ||
      _content_rect.height != content_rc.height) {
    _content_rect = content_rc;
    fix_content_rect();
  }
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

void window::enable_input(const int32_t &x, const int32_t &y) {
  uint32_t _x = x;
  uint32_t _y = y;
  if (_border.left) {
    _x++;
  }
  if (_border.top) {
    _y++;
  }
  auto xx = _x + _content_rect.x;
  auto yy = _y + _content_rect.y;
  wmove(stdscr, yy, xx);
  curs_set(1);
  echo();
}
void window::disable_input() {
  curs_set(0);
  noecho();
}