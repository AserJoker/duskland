#include "system/application.hpp"
#include "tui/document.hpp"
#include "tui/input.hpp"
#include "tui/layout_horizontal.hpp"
#include "tui/layout_vertical.hpp"
#include "tui/select.hpp"
#include "tui/text.hpp"
#include "tui/widget.hpp"
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
  _keyboard = core::singleton<keyboard>::get();
  _resource = core::singleton<resource>::get();
  _graphic = core::singleton<tui::graphic>::get();
  _colors = core::singleton<util::color>::get();
}
application::~application() {
  _root = nullptr;
  _graphic->uninitialize();
  _keyboard->uninitialize();
}
int application::run() {
  try {
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
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
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
  auto layout = new tui::layout_horizontal();
  layout->add_child(new tui::text(L"Label: "));
  auto input = new tui::input(L"demo", 13);
  layout->add_child(input);
  layout->add_child(new tui::text(L" Stuffix"));
  layout->add_child(new tui::select(L"select", {
                                                   {L"item1", L"item1"},
                                                   {L"item2", L"item2"},
                                                   {L"item3", L"item3"},
                                               }));
  _root->add_child(layout);
  _root->next_active();
  _root->request_update();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (_root != nullptr) {
    if (_root->on_input(cmd)) {
      return;
    }
  }
  if (cmd.name == "<esc>") {
    exit();
  }
}