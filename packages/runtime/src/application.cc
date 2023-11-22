#include "../include/application.hpp"
#include "tui/include/checkbox.hpp"
#include "tui/include/document.hpp"
#include "tui/include/input.hpp"
#include "tui/include/layout_horizontal.hpp"
#include "tui/include/layout_vertical.hpp"
#include "tui/include/list_select.hpp"
#include "tui/include/select.hpp"
#include "tui/include/text.hpp"
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
  _root = new tui::document();
  auto layout = new tui::layout_vertical();
  auto layout2 = new tui::layout_horizontal();
  layout2->get_attribute().offset.x = 1;
  layout2->get_attribute().offset.y = 1;
  layout2->get_attribute().border_left = true;
  layout2->get_attribute().border_right = true;
  layout2->get_attribute().border_top = true;
  layout2->get_attribute().border_bottom = true;
  layout->add_child(
      new tui::list_select(L"list-select", {
                                               {L"zh_CN", L"简体中文"},
                                               {L"en_US", L"English"},
                                               {L"Lang1", L"Lang1"},
                                               {L"Lang2", L"Lang2"},
                                               {L"Lang3", L"Lang3"},
                                               {L"Lang4", L"Lang4"},
                                               {L"Lang5", L"Lang5"},
                                               {L"Lang6", L"Lang6"},
                                               {L"Lang7", L"Lang7"},
                                           }));
  layout->add_child(new tui::checkbox(L"enable"));
  layout->add_child(
      new tui::list_select(L"list-select", {
                                               {L"zh_CN", L"简体中文"},
                                               {L"en_US", L"English"},
                                               {L"Lang1", L"Lang1"},
                                               {L"Lang2", L"Lang2"},
                                               {L"Lang3", L"Lang3"},
                                               {L"Lang4", L"Lang4"},
                                               {L"Lang5", L"Lang5"},
                                               {L"Lang6", L"Lang6"},
                                               {L"Lang7", L"Lang7"},
                                           }));
  layout2->add_child(layout);
  layout2->add_child(new tui::input(L"demo-text", 12));
  _root->add_child(layout2);
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