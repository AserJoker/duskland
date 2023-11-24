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
  auto windows = new tui::windows();
  auto root = windows->get_root();
  root->first = new tui::windows::node();
  root->second = new tui::windows::node();
  root->offset = 20;
  root->second->first = new tui::windows::node();
  root->second->second = new tui::windows::node();
  root->second->direction = tui::windows::node::HORIZONTAL;
  root->second->offset = -20;
  root->first->key = "key-1";
  root->first->identity = "demo-window";
  windows->set_root(root);
  _root = new tui::document();
  auto win = windows->get_window("demo-window");
  auto layout = new tui::layout_vertical();
  win->add_child(layout);
  layout->add_child(new tui::input(L"dinput", 12));
  layout->add_child(new tui::select(
      L"dinput", {{L"Chinese", L"中文"}, {L"English", L"English"}}));
  _root->add_child(windows);
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
  if (cmd.name == "<a>") {
    auto windows = (tui::windows *)_root.get();
    auto root = windows->get_root();
    std::swap(root->first, root->second);
    windows->set_root(root);
  }
}