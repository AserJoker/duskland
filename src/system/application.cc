#include "system/application.hpp"
#include "tui/window.hpp"
#include "tui/window_form.hpp"
#include <fmt/format.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
using namespace duskland::system;
using namespace duskland;
class widget_text : public tui::widget_base {
private:
  std::wstring _text;
  bool _hl;

public:
  void render(const core::auto_release<tui::window> &win) {
    auto &rc = get_rect();
    if (_hl) {
      win->write(rc.x, rc.y, _text.c_str(), WA_STANDOUT);
    } else {
      win->write(rc.x, rc.y, _text.c_str());
    }
  }
  widget_text(const std::wstring &text, const std::string &name)
      : tui::widget_base(name), _text(text), _hl(false) {}
  void on_command(int cmd) {
    if (cmd == '\t') {
      _hl = !_hl;
    }
  }
};
class window_demo : public tui::window_form {
public:
  window_demo() : tui::window_form({0, 0, 40, 10}, "dmeo-window") {
    auto text = new widget_text(L"文本组件测试", "text");
    get_widgets().push_back(text);
    set_active_widget(text);
  }
  void on_update() { tui::window_form::on_update(); }
  void on_command(int cmd) {
    tui::window_form::on_command(cmd);
    update();
  }
};

core::auto_release<tui::window_form> win = nullptr;
application::application() : _is_running(false) {
  _tui = core::singleton<tui::system_tui>::get();
  _layout = core::singleton<tui::layout>::get();
  _attribute = core::singleton<util::attribute>::get();
  win = nullptr;
}
application::~application() {
  win = nullptr;
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
  _layout->initialize();
  win = new window_demo;
  win->set_border({true, true, true, true});
  win->update();
  win->active();
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