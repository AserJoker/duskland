#include "system/application.hpp"
#include "tui/widget_text.hpp"
#include "tui/window.hpp"
#include "tui/window_form.hpp"
#include "util/event.hpp"
#include <fmt/format.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
using namespace duskland::system;
using namespace duskland;
class demo_window : public tui::window_form {
public:
  demo_window(const util::rect &rc, const std::string &name)
      : tui::window_form(rc, name) {
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->add_widget(new tui::widget_text("a", L"demo text"));
    get_form()->next_active();
  }
  bool on_command(int cmd,
                  const core::auto_release<tui::widget_base> &emitter) {
    if (cmd == EVENT_SELECT) {
      auto text = (tui::widget_text *)emitter.get();
      text->set_text(L"select text");
      update();
    }
    return tui::window_form::on_command(cmd, emitter);
  }
};
application::application() : _is_running(false) {
  _tui = core::singleton<tui::system_tui>::get();
  _layout = core::singleton<tui::layout>::get();
  _attribute = core::singleton<util::attribute>::get();
}
application::~application() {
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

  _attribute->initialize();
  _attribute->attr("tui.border.normal", COLOR_WHITE, COLOR_BLACK);
  _attribute->attr("tui.text.normal", COLOR_WHITE, COLOR_BLACK, WA_NORMAL);
  _attribute->attr("tui.text.focus", COLOR_WHITE, COLOR_BLACK, WA_STANDOUT);

  _tui->initialize();
  auto win = new demo_window({0, 0, 0, 0}, "root window");
  _layout->initialize(win);
}
const std::vector<std::string> &application::argv() const { return _args; }
void application::set_cursor_style(cursor_style style) { curs_set(style); }
void application::clear() { ::clear(); }
void application::command(int ch) {
  switch (ch) {
  case 'q':
    exit();
    break;
  case ERR:
    break;
  default:
    _tui->run_command(ch);
    break;
  }
}