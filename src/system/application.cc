#include "system/application.hpp"
#include "tui/layout_vertical.hpp"
#include "tui/text.hpp"
#include "tui/widget.hpp"
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
core::auto_release<tui::widget> w;
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
  _input = core::singleton<input>::get();
  _resource = core::singleton<resource>::get();
  _graphic = core::singleton<tui::graphic>::get();
}
application::~application() {
  w = nullptr;
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
      w->render(_graphic);
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
  w = new tui::widget;
  w->get_attribute().border.left = true;
  w->get_attribute().border.right = true;
  w->get_attribute().border.top = true;
  w->get_attribute().border.bottom = true;
  w->get_attribute().offset.x = 10;
  w->get_attribute().offset.y = 10;
  w->get_attribute().size.width = 6;
  w->get_attribute().size.height = 6;
  w->get_attribute().overflow = tui::attribute::FIXED;
  auto txt = new tui::text(L"aaaabbbbccccddddd");
  txt->set_auto_wrap(true);
  w->add_child(txt);
  w->request_update();
}
void application::on_command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  }
  if (cmd.name == "<esc>") {
    exit();
  }
  if (cmd.name == "<a>") {
    w->get_attribute().size.width--;
    w->request_update();
  }
  if (cmd.name == "<d>") {
    w->get_attribute().size.width++;
    w->request_update();
  }
}