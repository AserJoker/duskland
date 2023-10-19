#include "tui/system_wm.hpp"
#include "util/key.hpp"
using namespace duskland;
using namespace duskland::tui;
system_wm::system_wm() : _active_window(nullptr) {}
void system_wm::initialize() {}
void system_wm::uninitialize() {
  if (_active_window) {
    _active_window->on_dective();
    _active_window = nullptr;
  }
  while (!_windows.empty()) {
    auto &win = *_windows.begin();
    win->dispose();
  }
}
bool system_wm::on_command(const util::command &cmd) {
  if (cmd.raw[0] == KEY_RESIZE) {
    refresh();
  }
  if (_active_window) {
    return _active_window->on_command(cmd);
  }
  return false;
}
void system_wm::set_active_window(window *win) {
  if (_active_window != win) {
    if (_active_window) {
      _active_window->on_dective();
      _active_window->render();
    }
    _active_window = win;
    if (_active_window) {
      _active_window->on_active();
      _active_window->render();
    }
  }
}
void system_wm::refresh() {
  ::refresh();
  auto active = get_active_window();
  for (auto &win : _windows) {
    if (win != active) {
      win->render();
    }
  }
  if (active) {
    active->render();
  }
}
std::vector<window *> &system_wm::get_windows() { return _windows; }
window *system_wm::get_active_window() { return _active_window; }
void system_wm::next_active() {
  if (_windows.size() > 1) {
    for (auto it = _windows.begin(); it != _windows.end(); it++) {
      if (*it == _active_window) {
        auto next = it + 1;
        if (next == _windows.end()) {
          next = _windows.begin();
        }
        set_active_window(*next);
        break;
      }
    }
  }
}
window *system_wm::select(const std::string &name) {
  for (auto &win : _windows) {
    if (win->get_name() == name) {
      return win;
    }
  }
  return nullptr;
}

void system_wm::set_access_node(const int32_t &x, const int32_t &y,
                                const util::border_info &border) {
  util::layout_info *vinfo = nullptr;
  for (auto &info : _layout_info) {
    if (info.x == x && info.y == y) {
      vinfo = &info;
      break;
    }
  }
  if (!vinfo) {
    _layout_info.push_back({0, 0, 0, 0, x, y});
    vinfo = &*(_layout_info.end() - 1);
  }
  if (border.left) {
    vinfo->left++;
  }
  if (border.top) {
    vinfo->top++;
  }
  if (border.right) {
    vinfo->right++;
  }
  if (border.bottom) {
    vinfo->bottom++;
  }
}
const util::layout_info *system_wm::get_access_node(const int32_t &x,
                                                    const int32_t &y) {
  for (auto &info : _layout_info) {
    if (info.x == x && info.y == y) {
      return &info;
    }
  }
  return nullptr;
}
void system_wm::remove_access_node(const int32_t &x, const int32_t &y,
                                   const util::border_info &border) {
  for (auto it = _layout_info.begin(); it != _layout_info.end(); it++) {
    auto &info = *it;
    if (info.x == x && info.y == y) {
      if (border.left) {
        info.left--;
      }
      if (border.right) {
        info.right--;
      }
      if (border.top) {
        info.top--;
      }
      if (border.bottom) {
        info.bottom--;
      }
      if (info.top == 0 && info.left == 0 && info.right == 0 &&
          info.bottom == 0) {
        _layout_info.erase(it);
      }
      return;
    }
  }
}