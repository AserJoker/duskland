#include "../include/application.hpp"
#include "core/include/auto_release.hpp"
#include "tui/include/document.hpp"
#include "tui/include/widget.hpp"
#include <chrono>
#include <codecvt>
#include <curses.h>
#include <iostream>
#include <thread>
using namespace duskland::runtime;
using namespace duskland;

application::application() : _is_running(false) {
  _keyboard = core::singleton<keyboard>::get();
  _resource = core::singleton<resource>::get();
  _graphic = core::singleton<tui::graphic>::get();
  _brush = core::singleton<tui::brush>::get();
}
application::~application() { uninitialize(); }
void application::uninitialize() {
  _root = nullptr;
  if (_keyboard != nullptr) {
    _keyboard->uninitialize();
    _keyboard = nullptr;
  }
  if (_graphic != nullptr) {
    _graphic->uninitialize();
    _graphic = nullptr;
  }
}
int application::run() {
  _is_running = true;
  auto now = std::chrono::system_clock::now();
  while (_is_running) {
    std::vector<util::key> keys;
    if (_keyboard->read(keys)) {
      for (auto &key : keys) {
        on_command(key);
      }
    }
    _graphic->set_position({0, 0});
    if (_root != nullptr) {
      _root->render(_graphic);
    }
    if (!this->_graphic->present()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }
  }
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  _resource->load("data");
  auto keymap = _resource->query("system.keymap");
  auto attribute = _resource->query("system.attribute");
  auto symbol = _resource->query("system.symbol");
  _keyboard->load(std::string(keymap.begin(), keymap.end()));
  _graphic->initialize(_brush);
  _keyboard->initialize();
  _brush->load_attribute(std::string(attribute.begin(), attribute.end()));
  _brush->load_symbol(std::string(symbol.begin(), symbol.end()));
  _root = new tui::document();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (_root != nullptr) {
    if (_root->process(cmd)) {
      return;
    }
  }
  if (cmd.name == "<esc>") {
    exit();
  }
}