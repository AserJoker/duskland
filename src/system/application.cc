﻿#include "system/application.hpp"
#include "tui/widget_col.hpp"
#include "tui/widget_input.hpp"
#include "tui/widget_line.hpp"
#include "tui/widget_text.hpp"
#include "tui/window.hpp"
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
static int32_t fps = 0;
class demo_window : public tui::window_widget {
private:
  core::auto_release<tui::widget_col> _col;

  int32_t offset;
  std::vector<wint_t> codes;

public:
  demo_window(const util::rect &rc, const std::string &name)
      : tui::window_widget(rc, name), offset(0) {
    // _message = new tui::widget_text("label", L"中文测试");
    // _input = new tui::widget_input("input", 12);
    // auto line = new tui::widget_line("layout.line");
    // line->add_widget(_message.get());
    // line->add_widget(_input.get());
    // line->add_widget(new tui::widget_text("tail", L"中文tail"));
    // get_root() = line;
    // line->next_active();
    auto col = new tui::widget_col("col");
    col->add_widget(new tui::widget_text("t1", L"item1"));
    col->add_widget(new tui::widget_text("t1", L"item2"));
    col->add_widget(new tui::widget_text("t1", L"item3"));
    col->add_widget(new tui::widget_text("t1", L"item4"));
    col->add_widget(new tui::widget_text("t1", L"item5"));
    col->add_widget(new tui::widget_text("t1", L"item6"));
    col->add_widget(new tui::widget_text("t1", L"item7"));
    col->add_widget(new tui::widget_text("t1", L"item8"));
    col->add_widget(new tui::widget_text("t1", L"item9"));
    col->add_widget(new tui::widget_text("t1", L"item10"));
    col->add_widget(new tui::widget_text("t1", L"item11"));
    col->add_widget(new tui::widget_text("t1", L"item12"));
    col->add_widget(new tui::widget_text("t1", L"item13"));
    col->add_widget(new tui::widget_text("t1", L"item14"));
    get_root() = col;
    col->next_active();
    _col = col;
  }
};
application::application() : _is_running(false) {
  _tui = core::singleton<tui::system_tui>::get();
  _layout = core::singleton<tui::layout>::get();
  _config = core::singleton<util::config>::get();
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
  _layout->uninitialize();
  _tui->uninitialize();
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  for (auto index = 0; index < argc; index++) {
    _args.push_back(argv[index]);
  }
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
  _config->initialize();

  _config->attr("tui.border.normal", COLOR_WHITE, COLOR_BLACK);
  _config->attr("tui.text.normal", COLOR_WHITE, COLOR_BLACK,
                WA_NORMAL | WA_DIM);
  _config->attr("tui.text.focus", COLOR_WHITE, COLOR_BLACK);
  _config->attr("tui.input.normal", COLOR_WHITE, COLOR_BLACK,
                WA_NORMAL | WA_DIM);
  _config->attr("tui.input.focus", COLOR_WHITE, COLOR_BLACK);
  _config->attr("tui.input.cursor", COLOR_WHITE, COLOR_BLACK, WA_STANDOUT);

  _config->keymap("key.next", '\t');
  _config->keymap("key.last", KEY_BTAB);
  _config->keymap("key.select", '\n');
  _config->keymap("key.quit", 'q');
  _config->style("style.border.ls", L'│');
  _config->style("style.border.rs", L'│');
  _config->style("style.border.ts", L'─');
  _config->style("style.border.bs", L'─');
  _config->style("style.border.tl", L'┌');
  _config->style("style.border.tr", L'┐');
  _config->style("style.border.bl", L'└');
  _config->style("style.border.br", L'┘');
  _config->style("style.border.tlr", L'┬');
  _config->style("style.border.blr", L'┴');
  _config->style("style.border.ltb", L'├');
  _config->style("style.border.rtb", L'┤');
  _config->style("style.border.lrtb", L'┼');

  _tui->initialize();
  auto win = new demo_window({0, 0, 0, 0}, "root window");
  // win->set_border({false, false, false, false});
  _layout->initialize(win);
}
const std::vector<std::string> &application::argv() const { return _args; }
void application::set_cursor_style(cursor_style style) { curs_set(style); }
void application::clear() { ::clear(); }
void application::command(wint_t cmd) {
  if (cmd == ERR) {
    return;
  } else if (cmd == _config->keymap("key.quit")) {
    exit();
  } else if (cmd == KEY_RESIZE) {
    this->set_cursor_style(CUR_INVISIBLE);
    refresh();
    if (_layout->relayout()) {
      _tui->refresh();
    }
  } else {
    _tui->run_command(cmd);
  }
}