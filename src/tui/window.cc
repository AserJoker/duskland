#include "tui/window.hpp"
#include "core/singleton.hpp"
#include "tui/system_wm.hpp"
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <ncurses.h>
using namespace duskland::tui;
using namespace duskland;
window::window(const std::string &name)
    : _win(nullptr), _rect({0, 0, 0, 0}), _content_rect({0, 0, 0, 0}),
      _is_active(false), _name(name), _border({false, false, false, false}) {
  auto tui = core::singleton<tui::system_wm>::get();
  auto &windows = tui->get_windows();
  windows.push_back(this);
  _injector = core::singleton<util::injector>::get();
}
window::~window() {}

util::rect &window::get_rect() { return _rect; }
const util::rect &window::get_rect() const { return _rect; }
const std::string &window::get_name() const { return _name; }
const util::rect &window::get_content_rect() const { return _content_rect; }
void window::set_content_rect(const util::rect &content_rect) {
  _content_rect = content_rect;
  fix_content_rect();
}

void window::initialize(const util::rect &rect) {
  _rect = rect;
  set_border_info({true, true, true, true});
  set_content_rect(get_bound_rect());
  _win = newwin(rect.height, rect.width, rect.y, rect.x);
  on_initialize();
}
void window::dispose() {
  on_dispose();
  auto tui = core::singleton<tui::system_wm>::get();
  if (tui->get_active_window() == this) {
    tui->set_active_window(nullptr);
  }
  auto &windows = tui->get_windows();
  std::erase(windows, this);
  if (!windows.empty()) {
    tui->set_active_window(*windows.begin());
  }
  delwin(_win);
  _win = nullptr;
  tui->refresh();
}
void window::render() {
  if (_win) {
    clear();
    refresh();
    draw_border();
    refresh();
    on_render();
  }
}
void window::clear() {
  for (auto y = 0; y < _rect.height; y++) {
    for (auto x = 0; x < _rect.width; x++) {
      mvwaddch(_win, y, x, ' ');
    }
  }
  refresh();
}
void window::refresh() { wrefresh(_win); }
void window::active() {
  auto tui = core::singleton<tui::system_wm>::get();
  tui->set_active_window(this);
}

bool window::is_active() { return _is_active; }

void window::move_to(const int32_t &x, const int32_t &y) {
  _rect.x = x;
  _rect.y = y;
  delwin(_win);
  _win = newwin(_rect.height, _rect.width, _rect.y, _rect.x);
  auto tui = core::singleton<tui::system_wm>::get();
  tui->refresh();
}
void window::resize_to(const uint32_t &width, const uint32_t &height) {
  _rect.width = width;
  _rect.height = height;
  if (_rect.width < 10) {
    _rect.width = 15;
  }
  if (_rect.height < 3) {
    _rect.height = 3;
  }
  fix_content_rect();
  delwin(_win);
  _win = newwin(_rect.height, _rect.width, _rect.y, _rect.x);
  auto tui = core::singleton<tui::system_wm>::get();
  tui->refresh();
  on_resize();
}
void window::move(const int32_t &dx, const int32_t &dy) {
  move_to(_rect.x + dx, _rect.y + dy);
}
void window::resize(const int32_t &dw, const int32_t &dh) {
  resize_to(_rect.width + dw, _rect.height + dh);
}
void window::draw(const int32_t &x, const int32_t &y, const wchar_t &ch,
                  const chtype &attr) {
  auto xx = x + _content_rect.x;
  auto yy = y + _content_rect.y;
  auto rc = get_bound_rect();
  if (yy < 0 || xx < 0 || xx >= rc.width || yy >= rc.height) {
    return;
  }
  xx += rc.x;
  yy += rc.y;
  cchar_t cc = {0, {ch, 0}};
  wattron(_win, attr);
  mvwadd_wch(_win, yy, xx, &cc);
  wattroff(_win, attr);
}
void window::draw(const int32_t &x, const int32_t &y, const char &ch,
                  const chtype &attr) {
  auto xx = x + _content_rect.x;
  auto yy = y + _content_rect.y;
  auto rc = get_bound_rect();
  if (yy < 0 || xx < 0 || xx >= rc.width || yy >= rc.height) {
    return;
  }
  xx += rc.x;
  yy += rc.y;
  wattron(_win, attr);
  mvwaddch(_win, yy, xx, ch);
  wattroff(_win, attr);
}
void window::draw(const int32_t &x, const int32_t &y, const std::wstring &str,
                  const chtype &attr) {
  auto offset = 0;
  for (auto c : str) {
    draw(x + offset, y, c, attr);
    offset += wcwidth(c);
  }
}
void window::draw(const int32_t &x, const int32_t &y, const std::string &str,
                  const chtype &attr) {
  auto offset = 0;
  for (auto c : str) {
    draw(x + offset, y, c, attr);
    offset++;
  }
}

void window::draw_absolute(const int32_t &x, const int32_t &y,
                           const wchar_t &ch, const chtype &attr) {
  cchar_t cc = {0, {ch, 0}};
  wattron(_win, attr);
  mvwadd_wch(_win, y, x, &cc);
  wattroff(_win, attr);
}
void window::draw_absolute(const int32_t &x, const int32_t &y, const char &ch,
                           const chtype &attr) {
  wattron(_win, attr);
  mvwaddch(_win, y, x, ch);
  wattroff(_win, attr);
}
void window::draw_absolute(const int32_t &x, const int32_t &y,
                           const std::wstring &str, const chtype &attr) {
  auto offset = 0;
  for (auto c : str) {
    draw_absolute(x + offset, y, c, attr);
    offset += wcwidth(c);
  }
}
void window::draw_absolute(const int32_t &x, const int32_t &y,
                           const std::string &str, const chtype &attr) {
  auto offset = 0;
  for (auto c : str) {
    draw_absolute(x + offset, y, c, attr);
    offset++;
  }
}
void window::fix_content_rect() {
  auto rc = get_bound_rect();
  if (_content_rect.x > 0) {
    _content_rect.x = 0;
  }
  if (_content_rect.y > 0) {
    _content_rect.y = 0;
  }
  if (_content_rect.height < rc.height) {
    _content_rect.height = rc.height;
  }
  if (_content_rect.width < rc.width) {
    _content_rect.width = rc.width;
  }
  if (rc.height - _content_rect.y > _content_rect.height) {
    _content_rect.y = rc.height - _content_rect.height;
  }
  if (rc.width - _content_rect.x > _content_rect.width) {
    _content_rect.x = rc.width - _content_rect.width;
  }
}

void window::on_dective() { _is_active = false; }
void window::on_active() { _is_active = true; }
bool window::on_command(const util::key &cmd) {
  if (cmd.decode == KEY_RESIZE) {
    on_resize();
    return true;
  }
  return false;
}
void window::draw_scroll() {
  auto rc = get_bound_rect();
  auto scroll = fmt::format(L"line: {}/{} columns: {}/{}",
                            -_content_rect.y + rc.height, _content_rect.height,
                            -_content_rect.x + rc.width, _content_rect.width);
  draw_absolute(1, _rect.height - 1, scroll,
                _injector->attr("tui.scroll.normal"));
}
void window::on_render() {}
util::rect window::get_bound_rect() {
  auto rc = get_rect();
  rc.height--;
  if (_border.left) {
    rc.x++;
    rc.width--;
  }
  if (_border.right) {
    rc.width--;
  }
  if (_border.top) {
    rc.y++;
    rc.height--;
  }
  return rc;
}
void window::draw_border_node(const int32_t &x, const int32_t &y) {
  if (x == 0 && y == 0) {
    if (_border.top && _border.left) {
      draw_absolute(x, y, _injector->style("style.border.rb"),
                    _injector->attr("tui.border.normal"));
    }
    if (_border.top && !_border.left) {
      draw_absolute(x, y, _injector->style("style.border.ts"),
                    _injector->attr("tui.border.normal"));
    }
    if (!_border.top && _border.left) {
      draw_absolute(x, y, _injector->style("style.border.ls"),
                    _injector->attr("tui.border.normal"));
    }
    return;
  }
  if (x == 0 && y == _rect.height - 1) {
    if (_border.bottom && _border.left) {
      draw_absolute(x, y, _injector->style("style.border.tr"),
                    _injector->attr("tui.border.normal"));
    }
    if (_border.bottom && !_border.left) {
      draw_absolute(x, y, _injector->style("style.border.bs"),
                    _injector->attr("tui.border.normal"));
    }
    if (!_border.bottom && _border.left) {
      draw_absolute(x, y, _injector->style("style.border.ls"),
                    _injector->attr("tui.border.normal"));
    }
    return;
  }
  if (x == _rect.width - 1 && y == 0) {
    if (_border.top && _border.right) {
      draw_absolute(x, y, _injector->style("style.border.lb"),
                    _injector->attr("tui.border.normal"));
    }
    if (_border.top && !_border.right) {
      draw_absolute(x, y, _injector->style("style.border.ts"),
                    _injector->attr("tui.border.normal"));
    }
    if (!_border.top && _border.right) {
      draw_absolute(x, y, _injector->style("style.border.rs"),
                    _injector->attr("tui.border.normal"));
    }
    return;
  }
  if (x == _rect.width - 1 && y == _rect.height - 1) {
    if (_border.bottom && _border.right) {
      draw_absolute(x, y, _injector->style("style.border.tl"),
                    _injector->attr("tui.border.normal"));
    }
    if (_border.bottom && !_border.right) {
      draw_absolute(x, y, _injector->style("style.border.bs"),
                    _injector->attr("tui.border.normal"));
    }
    if (!_border.bottom && _border.right) {
      draw_absolute(x, y, _injector->style("style.border.rs"),
                    _injector->attr("tui.border.normal"));
    }
    return;
  }
  if (y == 0) {
    draw_absolute(x, y, _injector->style("style.border.ts"),
                  _injector->attr("tui.border.normal"));
    return;
  }
  if (y == _rect.height - 1) {
    draw_absolute(x, y, _injector->style("style.border.bs"),
                  _injector->attr("tui.border.normal"));
    return;
  }
  if (x == 0) {
    draw_absolute(x, y, _injector->style("style.border.ls"),
                  _injector->attr("tui.border.normal"));
    return;
  }
  if (x == _rect.width - 1) {
    draw_absolute(x, y, _injector->style("style.border.rs"),
                  _injector->attr("tui.border.normal"));
    return;
  }
}
void window::draw_border() {
  auto tui = core::singleton<tui::system_wm>::get();
  if (_border.top) {
    for (auto x = 1; x < _rect.width - 1; x++) {
      draw_border_node(x, 0);
    }
  }
  if (_border.bottom) {
    for (auto x = 1; x < _rect.width - 1; x++) {
      draw_border_node(x, _rect.height - 1);
    }
  }
  if (_border.left) {
    for (auto y = 1; y < _rect.height - 1; y++) {
      draw_border_node(0, y);
    }
  }
  if (_border.right) {
    for (auto y = 1; y < _rect.height - 1; y++) {
      draw_border_node(_rect.width - 1, y);
    }
  }
  draw_border_node(0, 0);
  draw_border_node(0, _rect.height - 1);
  draw_border_node(_rect.width - 1, 0);
  draw_border_node(_rect.width - 1, _rect.height - 1);
}
const util::border_info &window::get_border_info() const { return _border; };

void window::set_border_info(const util::border_info &info) {
  _border = info;
  auto tui = core::singleton<tui::system_wm>::get();
  util::border_info tl = {false, false, false, false};
  tl.right = info.top;
  tl.bottom = info.left;
  tui->set_access_node(_rect.x, _rect.y, tl);
  util::border_info tr = {false, false, false, false};
  tr.left = info.top;
  tr.bottom = info.right;
  tui->set_access_node(_rect.x + _rect.width - 1, _rect.y, tr);
  util::border_info bl = {false, false, false, false};
  bl.right = info.bottom;
  bl.top = info.right;
  tui->set_access_node(_rect.x, _rect.y + _rect.height - 1, bl);
  util::border_info br = {false, false, false, false};
  br.left = info.bottom;
  br.top = info.right;
  tui->set_access_node(_rect.x + _rect.width - 1, _rect.y + _rect.height - 1,
                       br);
  fix_content_rect();
}
void window::set_current_pos(const int32_t &x, const int32_t &y) {
  auto &crt = get_content_rect();
  auto rc = get_bound_rect();
  set_content_rect({crt.x - (x - (int32_t)rc.width / 2),
                    crt.y - (y - (int32_t)rc.height / 2), crt.width,
                    crt.height});
}
void window::on_dispose() {}
void window::on_initialize() {}
void window::on_resize() {}
void window::on_emit(const core::auto_release<widget> &win,
                     const std::string &event) {}