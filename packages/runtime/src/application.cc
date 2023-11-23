#include "../include/application.hpp"
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
  _attributes = core::singleton<tui::attribute>::get();
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
  auto color = _resource->query("system.color");
  _keyboard->load(std::string(keymap.begin(), keymap.end()));
  _graphic->initialize(_attributes);
  _keyboard->initialize();
  _attributes->load(std::string(color.begin(), color.end()));
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
  auto win = windows->get_window("demo-window");
  auto txt = new tui::text(L"text");
  txt->get_attribute().offset.y = 1;
  txt->request_update();
  win->add_child(txt);
  _root = windows;
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