#include "system/application.hpp"
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
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
  _input = core::singleton<input>::get();
  _resource = core::singleton<resource>::get();
  _graphic = core::singleton<tui::graphic>::get();
}
application::~application() {
  _graphic->uninitialize();
  _input->uninitialize();
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
  _graphic->initialize();
  _injector->initialize();
  _input->initialize();
  _resource->load("data");
  auto keymap = _resource->query("keyboard.keymap");
  _input->load(std::string(keymap.begin(), keymap.end()));
  this->_document = new tui::document();
  this->_document->next_active();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (!_document->on_input(cmd)) {
    if (cmd.name == "<esc>") {
      exit();
    }
  }
}