﻿#include "system/application.hpp"
#include "tui/column.hpp"
#include "tui/document.hpp"
#include "tui/fixcontent.hpp"
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
tui::fixcontent *fix = nullptr;
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
  _input = core::singleton<input>::get();
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
      std::vector<util::key> keys;
      if (_input->read(keys)) {
        for (auto &key : keys) {
          on_command(key);
        }
      }

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
  auto col = new tui::column();
  auto col1 = new tui::column();
  auto col2 = new tui::column();
  auto col3 = new tui::column();
  fix = new tui::fixcontent();
  fix->set_rect({10, 10, 30, 10});
  this->_document->add_child(fix);
  this->_document->add_child(col3);
  fix->add_child(col);
  col->add_child(col1);
  col->add_child(col2);
  auto text1 = new tui::text(L"item1");
  auto text2 = new tui::text(L"item2");
  auto text3 = new tui::text(L"item3");
  auto text4 = new tui::text(L"item4");
  auto text5 = new tui::text(L"item5");
  col1->add_child(text1);
  col1->add_child(text2);
  col2->add_child(text3);
  col2->add_child(text4);
  col3->add_child(text5);
  this->_document->next_active();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (!this->_document->on_input(cmd)) {
    if (cmd.name == "<esc>") {
      exit();
    }
    if (cmd.name == "<w>") {
      auto pos = fix->get_position();
      fix->set_position({pos.x, pos.y + 1});
    }
    if (cmd.name == "<s>") {
      auto pos = fix->get_position();
      fix->set_position({pos.x, pos.y - 1});
    }
    if (cmd.name == "<W>") {
      auto &rc = fix->get_rect();
      fix->set_rect({rc.x, rc.y, rc.width, rc.height + 1});
    }
    if (cmd.name == "<S>") {
      auto &rc = fix->get_rect();
      fix->set_rect({rc.x, rc.y, rc.width, rc.height - 1});
    }
  }
}