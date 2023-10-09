#include "tui/layout.hpp"
#include <list>
using namespace duskland::tui;
using namespace duskland;
layout::layout() { _tui = core::singleton<system_tui>::get(); }
layout::~layout() {}
core::auto_release<layout_item> layout::get_active_item() {
  std::list<core::auto_release<layout_item>> worker_list = {_root};
  auto active_window = _tui->get_active_window();
  while (worker_list.size()) {
    auto item = *worker_list.begin();
    worker_list.erase(worker_list.begin());
    if (item->_win != nullptr) {
      if (item->_win == active_window) {
        return item;
      }
    } else {
      worker_list.push_back(item->_first);
      worker_list.push_back(item->_second);
    }
  }
  return nullptr;
}
void layout::initialize() {
  auto win = new window({0, 0, getmaxx(stdscr), getmaxy(stdscr)}, "root");
  _root = new tui::layout_item("root", win);
  win->set_border({true, true, true, true});
  _tui->set_active_window(win);
}
void layout::uninitialize() { _root = nullptr; }
bool layout::relayout() {
  _root->_rect = {0, 0, getmaxx(stdscr), getmaxy(stdscr)};
  if (!_root->relayout()) {
    clear();
    mvprintw(0, 0, "window is too small");
    refresh();
    return false;
  }
  _root->referesh();
  return true;
}
core::auto_release<layout_item> layout::select(const std::string &name) const {
  std::list<core::auto_release<layout_item>> worker_list = {_root};
  while (worker_list.size()) {
    auto item = *worker_list.begin();
    if (item->_name == name) {
      return item;
    } else if (item->_win == nullptr) {
      worker_list.push_back(item->_first);
      worker_list.push_back(item->_second);
    }
  }
  return nullptr;
}
void layout::remove(const core::auto_release<layout_item> &item) {
  if (item != nullptr && item != _root) {
    auto parent = item->_parent;
    auto selector = parent->_first;
    if (item == selector) {
      selector = parent->_second;
    }
    item->_win = nullptr;
    selector->_rect = parent->_rect;
    if (parent->_parent) {
      if (parent->_parent->_first == parent) {
        parent->_parent->_first = selector;
      } else {
        parent->_parent->_second = selector;
      }
      selector->_parent = parent->_parent;
    } else {
      _root = selector;
      selector->_parent = nullptr;
    }
    _root->relayout();
    _root->referesh();
  }
}