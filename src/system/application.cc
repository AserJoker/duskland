#include "system/application.hpp"
#include "tui/window.hpp"
#include <fmt/format.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
using namespace duskland::system;
using namespace duskland;
core::auto_release<tui::window> win;
core::auto_release<tui::window> win2;
core::auto_release<tui::window> win3;
application::application() : _is_running(false) {
  _tui = core::singleton<tui::system_tui>::get();
  _layout = core::singleton<tui::layout>::get();
}
application::~application() {
  win = nullptr;
  win2 = nullptr;
  win3 = nullptr;
  clrtoeol();
  refresh();
  endwin();
}
int application::run() {
  try {
    _is_running = true;
    int x = getmaxx(stdscr);
    int y = getmaxy(stdscr);
    while (_is_running) {
      int cx = getmaxx(stdscr);
      int cy = getmaxy(stdscr);
      if (cx != x || cy != y) {
        x = cx;
        y = cy;
        refresh();
        if (_layout->relayout()) {
          _tui->refresh();
        }
      }
      auto ch = getch();
      this->command(ch);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  _layout->uninitialize();
  _tui->uninitialize();
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  for (auto index = 0; index < argc; index++) {
    _args.push_back(argv[index]);
  }
  initscr();
  noecho();
  raw();
  halfdelay(1);
  keypad(stdscr, TRUE);
  start_color();
  this->clear();
  this->set_cursor_style(CUR_INVISIBLE);
  refresh();
  _tui->initialize();
  init_pair(1, COLOR_RED, 0);
  init_pair(2, COLOR_WHITE, 0);
  init_pair(3, COLOR_MAGENTA, 0);
  _layout->initialize();
}
const std::vector<std::string> &application::argv() const { return _args; }
void application::set_cursor_style(cursor_style style) { curs_set(style); }
void application::clear() { ::clear(); }
void application::command(int ch) {
  switch (ch) {
  case 'q':
    exit();
    break;
  case 's': {
    auto item = _layout->get_active_item();
    item->split(item->get_name() + ".s");
    break;
  }
  case 'v': {
    auto item = _layout->get_active_item();
    item->vsplit(item->get_name() + ".v");
    break;
  }
  case 'd': {
    _layout->remove(_layout->get_active_item());
    break;
  }
  case '\t':
    _tui->next_active();
    break;
  case KEY_BTAB:
    _tui->last_active();
    break;
  case ERR:
    break;
  default:
    _tui->run_command(ch);
    break;
  }
}