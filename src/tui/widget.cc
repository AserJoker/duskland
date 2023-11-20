#include "tui/widget.hpp"
using namespace duskland::tui;
using namespace duskland;
widget *widget::_root = nullptr;
widget::widget()
    : _parent(nullptr), _is_changed(true), _update_lock(false),
      _active_widget(nullptr), _rect({0, 0, 0, 0}), _fixed_rect({0, 0, 0, 0}),
      _is_active(false) {}
void widget::emit(const std::string &event) { on_event(event, this); }
void widget::on_event(const std::string &event, widget *w) {
  if (event == "focus") {
    if (w != this) {
      if (_attr.yoverflow == attribute::SCROLL) {
        auto crc = w->get_bound_rect();
        if (crc.y < _rect.y) {
          set_focus({_fixed_rect.x, _fixed_rect.y + (_rect.y - crc.y)});
        }
        if (crc.y >= _rect.y + _rect.height) {
          set_focus({_fixed_rect.x,
                     _fixed_rect.y + (_rect.y + _rect.height - crc.y - 1)});
        }
        request_update();
      }
      if (_attr.xoverflow == attribute::SCROLL) {
        auto crc = w->get_bound_rect();
        if (crc.x < _rect.x) {
          set_focus({_fixed_rect.x + (_rect.x - crc.x), _fixed_rect.y});
        }
        if (crc.x >= _rect.x + _rect.width) {
          set_focus({_fixed_rect.x + (_rect.x + _rect.width - crc.x - 1),
                     _fixed_rect.y});
        }
        request_update();
      }
      if (_attr.xoverflow == attribute::SCROLL ||
          _attr.yoverflow == attribute::SCROLL) {
        return;
      }
    }
  }
  if (_parent) {
    _parent->on_event(event, w);
  }
}
void widget::on_render(core::auto_release<graphic> &g) {}
widget *widget::get_parent() { return _parent; }
std::vector<core::auto_release<widget>> &widget::get_children() {
  return _children;
}
void widget::render(core::auto_release<graphic> &g) {
  if (!_parent) {
    g->set_position({_rect.x, _rect.y});
    g->set_viewport({0, 0, 0, 0});
  }
  if (_is_changed) {
    clear(g);
    draw_border(g);
    draw_scroll(g);
    if (_is_active) {
      g->set_attr("active");
    } else {
      g->set_attr("normal");
    }
    on_render(g);
    _is_changed = false;
  }
  auto rc = g->get_viewport();
  for (auto &c : _children) {
    if (c->_attr.position == attribute::RELATIVE) {
      g->set_position({c->_rect.x, c->_rect.y});
      auto rect = _rect;
      if (rc.width || rc.height) {
        if (rect.x < rc.x) {
          rect.x = rc.x;
        }
        if (rect.y < rc.y) {
          rect.y = rc.y;
        }
        if (rect.x + rect.width > rc.x + rc.width) {
          rect.width = rc.x + rc.width - rect.x;
        }
        if (rect.y + rect.height > rc.y + rc.height) {
          rect.height = rc.y + rc.height - rect.y;
        }
      }
      g->set_viewport(rect);
    } else {
      g->set_position({c->_rect.x, c->_rect.y});
      g->set_viewport({0, 0, 0, 0});
    }
    c->render(g);
  }
}
void widget::clear(core::auto_release<graphic> &g) {
  g->set_attr(_attr.attr);
  for (auto x = 0; x < _rect.width; x++) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(x, y, L' ');
    }
  }
}

void widget::calculate_pos() {
  _rect.x = _attr.offset.x;
  _rect.y = _attr.offset.y;
  if (_attr.position == attribute::RELATIVE) {
    if (_parent) {
      _rect.x += _parent->_rect.x;
      _rect.x += _parent->_fixed_rect.x;
      _rect.y += _parent->_rect.y;
      _rect.y += _parent->_fixed_rect.y;
    }
  }
}

void widget::calculate_width() {
  util::rect rc = _rect;
  _rect.width = _attr.size.width;
  if (_rect.width == -1) {
    if (_parent) {
      _rect.width = _parent->_rect.width;
    }
  }
  if (_attr.xoverflow == attribute::SCROLL) {
    for (auto &c : _children) {
      if (c->_attr.position == attribute::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.x + rc.width > _fixed_rect.width + _fixed_rect.x) {
          _fixed_rect.width = rc.x + rc.width - _rect.x - _fixed_rect.x;
        }
      }
    }
  }
  if (_attr.xoverflow == attribute::VISIBLE) {
    for (auto &c : _children) {
      if (c->_attr.position == attribute::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.x + rc.width > _rect.width + _rect.x) {
          _rect.width = rc.x + rc.width - _rect.x - _fixed_rect.x;
        }
      }
    }
    _fixed_rect.x = 0;
    _fixed_rect.width = _rect.width;
  }
  if (_attr.max_size.width && _rect.width > _attr.max_size.width) {
    _rect.width = _attr.max_size.width;
  }
  calculate_fixed();
}
void widget::relayout() {
  if (_attr.position == attribute::ABSOLUTE && widget::_root != this) {
    if (widget::_root) {
      widget::_root->request_update();
    }
  } else {
    if (_parent) {
      _parent->request_update();
    }
  }
}
void widget::calculate_height() {
  auto rc = _rect;
  _rect.height = _attr.size.height;
  if (_rect.height == -1) {
    if (_parent) {
      _rect.height = _parent->_rect.height;
    }
  }
  if (_attr.yoverflow == attribute::SCROLL) {
    for (auto &c : _children) {
      if (c->_attr.position == attribute::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.y + rc.height > _fixed_rect.height + _fixed_rect.y) {
          _fixed_rect.height = rc.y + rc.height - _rect.y - _fixed_rect.y;
        }
      }
    }
  }
  if (_attr.yoverflow == attribute::VISIBLE) {
    for (auto &c : _children) {
      if (c->_attr.position == attribute::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.y + rc.height > _rect.height + _rect.y) {
          _rect.height = rc.y + rc.height - _rect.y - _fixed_rect.y;
        }
      }
    }
    _fixed_rect.y = 0;
    _fixed_rect.height = _rect.height;
  }
  if (_attr.max_size.height && _rect.height > _attr.max_size.height) {
    _rect.height = _attr.max_size.height;
  }
  calculate_fixed();
}
void widget::request_update() {
  _is_changed = true;
  if (_update_lock) {
    return;
  }
  _update_lock = true;

  on_update();
  calculate_pos();
  auto rc = _rect;
  if (_attr.xoverflow == attribute::FIXED) {
    calculate_width();
  }
  if (_attr.yoverflow == attribute::FIXED) {
    calculate_height();
  }
  for (auto &c : _children) {
    c->request_update();
  }
  if (_attr.xoverflow == attribute::SCROLL ||
      _attr.xoverflow == attribute::VISIBLE) {
    calculate_width();
  }
  if (_attr.yoverflow == attribute::SCROLL ||
      _attr.yoverflow == attribute::VISIBLE) {
    calculate_height();
  }
  calculate_fixed();
  if (rc.width != _rect.width || rc.height != _rect.height) {
    relayout();
  }
  _update_lock = false;
}
bool widget::on_input(const util::key &key) {
  if (_children.size()) {
    if (_active_widget) {
      if (_active_widget->on_input(key)) {
        return true;
      }
    }
    if (key.name == "<tab>") {
      return next_active();
    }
  }
  return false;
}
void widget::draw_border(core::auto_release<graphic> &g) {
  g->set_attr(_attr.border.attr);
  if (_attr.border.left) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(-1, y, L'│');
    }
  }
  if (_attr.border.right) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(_rect.width, y, L'│');
    }
  }
  if (_attr.border.top) {
    for (auto x = 0; x < _rect.width; x++) {
      g->draw(x, -1, L'─');
    }
  }
  if (_attr.border.bottom) {
    for (auto x = 0; x < _rect.width; x++) {
      g->draw(x, _rect.height, L'─');
    }
  }

  if (_attr.border.left && _attr.border.top) {
    g->draw(-1, -1, L'┌');
  }
  if (_attr.border.right && _attr.border.top) {
    g->draw(_rect.width, -1, L'┐');
  }
  if (_attr.border.left && _attr.border.bottom) {
    g->draw(-1, _rect.height, L'└');
  }
  if (_attr.border.right && _attr.border.bottom) {
    g->draw(_rect.width, _rect.height, L'┘');
  }

  if (_attr.border.left && !_attr.border.top) {
    g->draw(-1, -1, L'│');
  }
  if (!_attr.border.left && _attr.border.top) {
    g->draw(-1, -1, L'─');
  }

  if (_attr.border.right && !_attr.border.top) {
    g->draw(_rect.width, -1, L'│');
  }
  if (!_attr.border.right && _attr.border.top) {
    g->draw(_rect.width, -1, L'─');
  }

  if (_attr.border.left && !_attr.border.bottom) {
    g->draw(-1, _rect.height - 1, L'│');
  }
  if (!_attr.border.left && _attr.border.bottom) {
    g->draw(-1, _rect.height, L'─');
  }

  if (_attr.border.right && !_attr.border.bottom) {
    g->draw(_rect.width, _rect.height, L'│');
  }
  if (!_attr.border.right && _attr.border.bottom) {
    g->draw(_rect.width, _rect.height, L'─');
  }
}
void widget::on_update() {}
util::rect widget::get_bound_rect() {
  calculate_pos();
  calculate_width();
  calculate_height();
  util::rect rc = _rect;
  if (_attr.border.left) {
    rc.x--;
    rc.width++;
  }
  if (_attr.border.right) {
    rc.width++;
  }
  if (_attr.border.top) {
    rc.y--;
    rc.height++;
  }
  if (_attr.border.bottom) {
    rc.height++;
  }
  return rc;
}
attribute &widget::get_attribute() { return _attr; }
void widget::add_child(const core::auto_release<widget> &w) {
  _children.push_back(w);
  w->_parent = this;
  request_update();
}
void widget::remove_child(const core::auto_release<widget> &w) {
  for (auto it = _children.begin(); it != _children.end(); it++) {
    if (*it == w) {
      if (w == _active_widget) {
        next_active();
        if (w == _active_widget) {
          set_active(nullptr);
        }
      }
      w->_parent = nullptr;
      _children.erase(it);
      break;
    }
  }
  request_update();
}
void widget::draw_scroll(core::auto_release<graphic> &g) {
  g->set_attr(_attr.attr_scroll);
  if (_fixed_rect.height > _rect.height) {
    if (_fixed_rect.height - _rect.height < _rect.height) {
      auto scroll = _rect.height - (_fixed_rect.height - _rect.height);
      for (auto y = 0; y < scroll; y++) {
        g->draw(-1, y - _fixed_rect.y, L' ');
      }
    } else {
      auto offset = (-_fixed_rect.y) * 1.0 /
                    (_fixed_rect.height - _rect.height) * (_rect.height - 1);
      g->draw(-1, offset, L' ');
    }
  }

  if (_fixed_rect.width > _rect.width) {
    if (_fixed_rect.width - _rect.width < _rect.width) {
      auto scroll = _rect.width - (_fixed_rect.width - _rect.width);
      for (auto x = 0; x < scroll; x++) {
        g->draw(x - _fixed_rect.x, _rect.height, L' ');
      }
    } else {
      auto offset = (-_fixed_rect.x) * 1.0 / (_fixed_rect.width - _rect.width) *
                    (_rect.width - 1);
      g->draw(offset, _rect.height, L' ');
    }
  }
}

void widget::set_focus(const util::point &pos) {
  _fixed_rect.x = pos.x;
  _fixed_rect.y = pos.y;
  calculate_fixed();
  request_update();
}
util::point widget::get_focus() const { return {_fixed_rect.x, _fixed_rect.y}; }
void widget::calculate_fixed() {
  if (_fixed_rect.x > 0) {
    _fixed_rect.x = 0;
  }
  if (_fixed_rect.y > 0) {
    _fixed_rect.y = 0;
  }
  if (_fixed_rect.width > _rect.width) {
    if (_fixed_rect.width + _fixed_rect.x < _rect.width) {
      _fixed_rect.x = _rect.width - _fixed_rect.width;
    }
  } else {
    _fixed_rect.x = 0;
  }
  if (_fixed_rect.height > _rect.height) {
    if (_fixed_rect.height + _fixed_rect.y < _rect.height) {
      _fixed_rect.y = _rect.height - _fixed_rect.height;
    }
  } else {
    _fixed_rect.y = 0;
  }
}
const util::rect &widget::get_rect() const { return _rect; }
void widget::on_active() {
  if (_active_widget) {
    _active_widget->on_active();
  } else {
    next_active();
  }
  _is_active = true;
  request_update();
  emit("focus");
}
void widget::on_dective() {
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _is_active = false;
  request_update();
}
void widget::set_active(widget *w) {
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _active_widget = w;
  if (_active_widget) {
    _active_widget->on_active();
  }
}
widget *widget::get_active() { return _active_widget; }
bool widget::next_active() {
  if (_children.empty()) {
    return false;
  }
  if (!_active_widget) {
    for (auto &c : _children) {
      if (c->_attr.selectable) {
        set_active(c.get());
        return true;
      }
    }
  } else {
    for (auto it = _children.begin(); it != _children.end(); it++) {
      if (*it == _active_widget) {
        auto next = it + 1;
        while (next != _children.end()) {
          if ((*next)->_attr.selectable) {
            break;
          }
        }
        if (next == _children.end()) {
          set_active(nullptr);
          return false;
        } else {
          set_active(next->get());
          return true;
        }
      }
    }
  }
  return false;
}
bool widget::is_active() { return _is_active; }