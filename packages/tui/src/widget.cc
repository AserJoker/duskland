#include "../include/widget.hpp"
#include "core/include/auto_release.hpp"
#include "util/include/key.hpp"
using namespace duskland;
using namespace duskland::tui;
widget::widget()
    : _size({0, 0}), _offset({0, 0}), _parent(nullptr), _active(nullptr) {}
void widget::render(core::auto_release<graphic> &g) {
  auto &pos = g->get_position();
  for (auto &c : _children) {
    g->set_position({pos.x + c->_offset.x, pos.y + c->_offset.y});
    c->render(g);
  }
}
bool widget::process(const util::key& cmd) { return false; }
void widget::relayout() {
  for (auto &c : _children) {
    c->relayout();
  }
}
void widget::add_child(const core::auto_release<widget> &child) {
  _children.push_back(child);
  child->_parent = this;
  relayout();
}
void widget::remove_child(const core::auto_release<widget> &child) {
  std::erase(_children, child);
  child->_parent = nullptr;
  relayout();
}

const std::vector<core::auto_release<widget>> &widget::get_children() const {
  return _children;
};
widget *widget::get_parent() { return _parent; };

const util::size &widget::get_size() const { return _size; }
const util::point &widget::get_offset() const { return _offset; }
void widget::set_size(const util::size &size) {
  if (_size.width != size.width || _size.height != size.height) {
    _size = size;
    if (_parent) {
      _parent->relayout();
    } else {
      relayout();
    }
  }
}
void widget::set_offset(const util::point &offset) {
  if (_offset.x != offset.x || _offset.y != offset.y) {
    _offset = offset;
    if (_parent) {
      _parent->relayout();
    }
  }
}
void widget::emit(const std::string &event) { on(event, this); }
void widget::on(const std::string &event, widget *emitter) {
  if (_parent) {
    _parent->on(event, emitter);
  }
}