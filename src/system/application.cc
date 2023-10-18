#include "system/application.hpp"
#include "tui/widget_col.hpp"
#include "tui/widget_text.hpp"
#include "tui/window_widget.hpp"
#include "util/event.hpp"
#include <chrono>
#include <codecvt>
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
#include <thread>
using namespace duskland::system;
using namespace duskland;
class window_demo : public tui::window_widget {
public:
  void on_initialize() override {
    tui::window_widget::on_initialize();
    auto col1 = new tui::widget_col(get_name() + ".col");
    for (int i = 0; i < 5; i++) {
      auto text = new tui::widget_text(fmt::format("text.{}", i),
                                       fmt::format(L"item{}", i));
      col1->add_widget(text);
    }
    auto col2 = new tui::widget_col(get_name() + ".col2");
    for (int i = 0; i < 5; i++) {
      auto text = new tui::widget_text(fmt::format("text.{}", i),
                                       fmt::format(L"item{}", i));
      col2->add_widget(text);
    }
    auto col = new tui::widget_col(get_name() + ".col3");
    col->add_widget(col1);
    col->add_widget(col2);
    get_root() = col;
    col->next_active();
    render();
  }
  window_demo() : tui::window_widget("demo window") {}
};
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
  _wm = core::singleton<tui::system_wm>::get();
  _win = new window_demo();
}
application::~application() {
  clrtoeol();
  refresh();
  endwin();
}
int application::run() {
  try {
    _is_running = true;
    while (_is_running) {
      auto ch = getch();
      this->command(ch);
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  _wm->uninitialize();
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  raw();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  set_escdelay(1);
  this->clear();
  this->set_cursor_style(CUR_INVISIBLE);
  refresh();
  _injector->initialize();

  for (auto index = 0; index < argc; index++) {
    _injector->args().push_back(argv[index]);
  }
  _injector->attr("tui.border.normal", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.scroll.normal", COLOR_BLUE, COLOR_BLACK);
  _injector->attr("tui.text.normal", COLOR_WHITE, COLOR_BLACK, WA_DIM);
  _injector->attr("tui.text.focus", COLOR_RED, COLOR_BLACK);
  _injector->attr("tui.input.normal", COLOR_WHITE, COLOR_BLACK,
                  WA_NORMAL | WA_DIM);
  _injector->attr("tui.input.focus", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.input.cursor", COLOR_WHITE, COLOR_BLACK, WA_STANDOUT);

  _injector->keymap("key.next", '\t');
  _injector->keymap("key.last", KEY_BTAB);
  _injector->keymap("key.select", '\n');
  _injector->keymap("key.quit", 'q');
  _injector->style("style.border.ls", L'│');
  _injector->style("style.border.rs", L'│');
  _injector->style("style.border.ts", L'─');
  _injector->style("style.border.bs", L'─');
  _injector->style("style.border.rb", L'┌');
  _injector->style("style.border.lb", L'┐');
  _injector->style("style.border.tr", L'└');
  _injector->style("style.border.tl", L'┘');
  _injector->style("style.border.tlr", L'┬');
  _injector->style("style.border.blr", L'┴');
  _injector->style("style.border.ltb", L'├');
  _injector->style("style.border.rtb", L'┤');
  _injector->style("style.border.lrtb", L'┼');

  _wm->initialize();
  _win->initialize(
      {0, 0, (uint32_t)getmaxx(stdscr), (uint32_t)getmaxy(stdscr)});
  _win->active();
  _win->render();
}
void application::set_cursor_style(cursor_style style) { curs_set(style); }
void application::clear() { ::clear(); }
void application::command(wint_t cmd) {
  if (cmd == ERR) {
    return;
  } else if (cmd == _injector->keymap("key.quit")) {
    exit();
  } else {
    _wm->on_command(cmd);
  }
}