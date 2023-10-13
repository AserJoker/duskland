#include "tui/system_tui.hpp"
#include "util/config.hpp"
using namespace duskland::tui;
using namespace duskland;
void system_tui::initialize() {}
void system_tui::uninitialize() { _windows.clear(); }
void system_tui::run_command(int ch) {
  if (_active_window) {
    _active_window->on_command(ch, nullptr);
  }
}
window *system_tui::get_active_window() { return _active_window; }
void system_tui::set_active_window(window *win) {
  if (_active_window != win) {
    if (_active_window) {
      _active_window->on_dective();
    }
    _active_window = win;
  }
  win->on_active();
}
system_tui::system_tui() : _active_window(nullptr) {}
void system_tui::add_window(window *win) {
  if (std::find(_windows.begin(), _windows.end(), win) == _windows.end()) {
    _windows.push_back(win);
  }
}
void system_tui::remove_window(window *win) {
  for (auto it = _windows.begin(); it != _windows.end(); it++) {
    if (*it == win) {
      if (win == _active_window) {
        if (_windows.size() > 1) {
          auto next = it + 1;
          if (next == _windows.end()) {
            next = _windows.begin();
          }
          set_active_window(*next);
        } else {
          _active_window = nullptr;
        }
      }
      _windows.erase(it);
      break;
    }
  }
}
void system_tui::refresh() {
  for (auto &win : _windows) {
    win->refresh();
  }
  _active_window->on_active();
}
void system_tui::next_active() {
  if (_windows.size() > 1) {
    auto it = std::find(_windows.begin(), _windows.end(), _active_window);
    ++it;
    if (it == _windows.end()) {
      it = _windows.begin();
    }
    set_active_window(*it);
  }
}
void system_tui::last_active() {
  if (_windows.size() > 1) {
    auto it = std::find(_windows.begin(), _windows.end(), _active_window);
    if (it == _windows.begin()) {
      it = _windows.end() - 1;
    } else {
      --it;
    }
    set_active_window(*it);
  }
}