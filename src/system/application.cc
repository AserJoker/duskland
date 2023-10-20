#include "system/application.hpp"
#include "tui/widget_col.hpp"
#include "tui/widget_input.hpp"
#include "tui/widget_line.hpp"
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
    auto line = new tui::widget_line("line");
    auto text1 = new tui::widget_text("label", L"输入测试:");
    auto input = new tui::widget_input("input", 12);
    auto text2 = new tui::widget_text("tail", L"中文Tail");
    line->add_widget(text1);
    line->add_widget(input);
    line->add_widget(text2);
    line->next_active();
    get_root() = line;
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
      read_command();
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
  _injector->attr("tui.text.focus", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.input.normal", COLOR_WHITE, COLOR_BLACK,
                  WA_NORMAL | WA_DIM);
  _injector->attr("tui.input.focus", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.input.cursor", COLOR_WHITE, COLOR_BLACK, WA_STANDOUT);

  _injector->keymap("key.next", VKEY_TAB);
  _injector->keymap("key.last", VKEY_SHIFT_TAB);
  _injector->keymap("key.select", VKEY_ENTER);
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
void application::command(const util::command &cmd) {
  if (cmd.raw.empty()) {
    return;
  } else if (cmd.raw[0] == _injector->keymap("key.quit")) {
    exit();
  } else {
    _wm->on_command(cmd);
  }
}
void application::read_command() {
  std::vector<wint_t> codes;
  for (;;) {
    wint_t c;
    if (_injector->feature("feature.text_input")) {
      if (get_wch(&c) == ERR) {
        c = ERR;
      }
    } else {
      c = getch();
    }
    if (c == ERR) {
      break;
    }
    codes.push_back(c);
  }
  decode_command(codes);
}
void application::decode_command(const std::vector<wint_t> &codes) {
  util::command cmd = {codes, 0};
  if (!codes.empty()) {
    for (auto &[decode, raw] : util::key_code_mapping) {
      if (raw.size() == codes.size() &&
          std::equal(raw.begin(), raw.end(), codes.begin(), codes.end())) {
        cmd.decode = decode;
        return command(cmd);
      }
    }
    auto offset = 0;
    for (auto &code : codes) {
      command({{code}, code});
      char str[1024];
      sprintf(str, "0x%x", code);
      mvprintw(10 + offset, 10, str);
      offset++;
    }
    refresh();
  }
}