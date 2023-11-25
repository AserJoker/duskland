#include "../include/application.hpp"
#include "tui/include/checkbox.hpp"
#include "tui/include/document.hpp"
#include "tui/include/input.hpp"
#include "tui/include/layout_vertical.hpp"
#include "tui/include/select.hpp"
#include "tui/include/text.hpp"
#include "tui/include/windows.hpp"
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

  _system_scene = core::singleton<game::system_scene>::get();
  _system_entity = core::singleton<game::system_entity>::get();
  _system_map = core::singleton<game::system_map>::get();
  _system_hud = core::singleton<game::system_hud>::get();
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
  _system_map->initialize();
  _system_entity->initialize();
  _system_hud->initialize();
  _system_scene->initialize(_root);
  _root->next_active();
  _root->request_update();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (_root != nullptr) {
    if (_root->process_input(cmd)) {
      return;
    }
  }
  if (cmd.name == "<esc>") {
    exit();
  }
}