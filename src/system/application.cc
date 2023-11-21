#include "system/application.hpp"
#include "tui/checkbox.hpp"
#include "tui/document.hpp"
#include "tui/input.hpp"
#include "tui/layout_horizontal.hpp"
#include "tui/layout_vertical.hpp"
#include "tui/list_select.hpp"
#include "tui/select.hpp"
#include "tui/text.hpp"
#include "tui/widget.hpp"
#include <chrono>
#include <codecvt>
#include <curses.h>
#include <iostream>
#include <thread>
using namespace duskland::system;
using namespace duskland;
application::application() : _is_running(false) {
  _keyboard = core::singleton<keyboard>::get();
  _resource = core::singleton<resource>::get();
  _graphic = core::singleton<tui::graphic>::get();
  _colors = core::singleton<util::color>::get();
  _script = core::singleton<script::runtime>::get();
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
  _graphic->initialize(_colors);
  _keyboard->initialize();
  _colors->load(std::string(color.begin(), color.end()));
  _root = new tui::document();
  auto layout = new tui::layout_vertical();
  auto layout2 = new tui::layout_horizontal();
  layout2->get_attribute().offset.x = 1;
  layout2->get_attribute().offset.y = 1;
  layout2->get_attribute().border = {true, true, true, true};
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