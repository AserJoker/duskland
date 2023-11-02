#include "system/application.hpp"
#include "tui/column.hpp"
#include "tui/document.hpp"
#include "tui/line.hpp"
#include "tui/text.hpp"
#include "util/event.hpp"
#include <chrono>
#include <codecvt>
#include <curses.h>
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <iostream>
#include <thread>
using namespace duskland::system;
using namespace duskland;
tui::column *col1 = nullptr;
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
}
application::~application() {
  clrtoeol();
  refresh();
}
int application::run() {
  try {
    _is_running = true;
    auto now = std::chrono::system_clock::now();
    while (_is_running) {
      read_command();
      this->_document->render(this->_graphic);
      if (!this->_graphic->present()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
      }
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  _graphic = new tui::graphic();
  _injector->initialize();

  nodelay(stdscr, TRUE);
  set_escdelay(1);

  this->_document = new tui::document();
  auto line1 = new tui::line();
  col1 = new tui::column();
  auto col2 = new tui::column();
  this->_document->add_child(line1);
  line1->add_child(col1);
  line1->add_child(col2);
  auto text1 = new tui::text(L"item1");
  auto text2 = new tui::text(L"item2");
  auto text3 = new tui::text(L"item3");
  auto text4 = new tui::text(L"item4");
  col1->add_child(text1);
  col1->add_child(text2);
  col2->add_child(text3);
  col2->add_child(text4);
  this->_document->next_active();
  this->_document->request_update();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (!this->_document->on_input(cmd)) {
    if (cmd.name() == "<esc>") {
      exit();
    }
    if (cmd.name() == "<a>") {
      col1->set_border({true, true, true, true});
    }
    if (cmd.name() == "<d>") {
      col1->set_border({false, false, false, false});
    }
  }
}
void application::read_command() {
  std::vector<wint_t> codes;
  for (;;) {
    wint_t c;
    if (get_wch(&c) == ERR) {
      c = ERR;
    }
    if (c == ERR) {
      break;
    }
    codes.push_back(c);
  }
  decode_command(codes);
}
void application::decode_command(const std::vector<wint_t> &codes) {
  util::key cmd = {0, false, false, false, codes};
  if (!codes.empty()) {
    auto k = util::keymap;
    for (auto i = 0; i < codes.size(); i++) {
      k = k[codes[i]];
    }
    if (k.code().decode != 0) {
      cmd = k.code();
      cmd.raw = codes;
      on_command(cmd);
    } else {
      for (auto &code : codes) {
        cmd.raw = {code};
        if (code >= 0x1 && code <= 0x18) {
          cmd.decode = code + 'a';
          cmd.shift = false;
          cmd.ctrl = true;
          cmd.alt = false;
        } else {
          cmd.decode = code;
          cmd.shift = false;
          cmd.ctrl = false;
          cmd.alt = false;
        }
        on_command(cmd);
      }
    }
  }
}