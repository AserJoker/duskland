#include "tui/layout_item.hpp"
#include "core/singleton.hpp"
#include "tui/layout.hpp"
using namespace duskland::tui;
using namespace duskland;
layout_item::layout_item(const std::string &name,
                         const core::auto_release<window> &win)
    : _win(win), _parent(nullptr), _split_size(0), _vertical(false),
      _name(name) {
  _rect = win->get_rect();
}
layout_item::~layout_item() {}
core::auto_release<window> layout_item::get_window() { return _win; }
const std::string &layout_item::get_name() const { return _name; }
void layout_item::split(const core::auto_release<window> &win,
                        const int32_t &size) {
  if (_rect.width < 20) {
    return;
  }
  auto new_width = size;
  _split_size = size;
  if (new_width == 0) {
    new_width = _rect.width / 2;
  }
  if (new_width < 0) {
    new_width = _rect.width + new_width;
  }
  _win->set_rect({_rect.x, _rect.y, (uint32_t)new_width, _rect.height});
  _win->set_border({true, true, true, true});
  _first = new layout_item(_name, _win);
  win->set_rect({_rect.x + new_width - 1, _rect.y, _rect.width - new_width + 1,
                 _rect.height});
  win->set_border({true, true, true, true});
  _second = new layout_item(win->get_name(), win);

  _win = nullptr;
  _name = "";
  _first->_parent = this;
  _second->_parent = this;
  win->update();
  win->active();
}
void layout_item::vsplit(const core::auto_release<window> &win,
                         const int32_t &size) {
  if (_rect.height < 6) {
    return;
  }
  auto new_height = size;
  _split_size = size;
  _vertical = true;
  if (new_height == 0) {
    new_height = _rect.height / 2;
  }
  if (new_height < 0) {
    new_height = _rect.height + new_height;
  }
  core::auto_release<layout_item> selector = nullptr;
  if (_parent) {
    selector = _parent->_first;
    if (selector == this) {
      selector = _parent->_second;
    }
  }
  _win->set_rect({_rect.x, _rect.y, _rect.width, (uint32_t)new_height});
  _first = new layout_item(_name, _win);
  win->set_rect({_rect.x, _rect.y + new_height - 1, _rect.width,
                 _rect.height - new_height + 1});
  _second = new layout_item(win->get_name(), win);

  _win = nullptr;
  _name = "";
  _first->_parent = this;
  _second->_parent = this;
  win->update();
  win->active();
}
void layout_item::referesh() {
  if (_win != nullptr) {
    _win->set_rect(_rect);
    _win->refresh();
  } else {
    _first->referesh();
    _second->referesh();
  }
}
bool layout_item::relayout() {
  if (_win == nullptr) {
    if (_vertical) {
      auto new_height = _split_size;
      if (new_height == 0) {
        new_height = _rect.height / 2;
      }
      if (new_height < 0) {
        new_height = _rect.height + new_height;
      }
      if (new_height < 3 || _rect.height - new_height < 3) {
        return false;
      }
      _first->_rect.x = _rect.x;
      _first->_rect.y = _rect.y;
      _first->_rect.height = new_height;
      _first->_rect.width = _rect.width;
      _second->_rect.x = _rect.x;
      _second->_rect.y = _rect.y + new_height - 1;
      _second->_rect.height = _rect.height - new_height + 1;
      _second->_rect.width = _rect.width;
      if (!_first->relayout() || !_second->relayout()) {
        return false;
      }
    } else {
      auto new_width = _split_size;
      if (new_width == 0) {
        new_width = _rect.width / 2;
      }
      if (new_width < 0) {
        new_width += _rect.width;
      }
      if (new_width < 10 || _rect.width - new_width < 10) {
        return false;
      }
      _first->_rect.x = _rect.x;
      _first->_rect.y = _rect.y;
      _first->_rect.width = new_width;
      _first->_rect.height = _rect.height;
      _second->_rect.x = _rect.x + new_width - 1;
      _second->_rect.y = _rect.y;
      _second->_rect.width = _rect.width - new_width + 1;
      _second->_rect.height = _rect.height;
      if (!_first->relayout() || !_second->relayout()) {
        return false;
      }
    }
  }
  return true;
}
void layout_item::set_name(const std::string &name) { _name = name; }