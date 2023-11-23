#include "../include/widget.hpp"
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
      if (_style.yoverflow == style::SCROLL) {
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
      if (_style.xoverflow == style::SCROLL) {
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
      if (_style.xoverflow == style::SCROLL ||
          _style.yoverflow == style::SCROLL) {
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
    if (c->_style.position == style::RELATIVE) {
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
  if (!_timers.empty()) {
    std::vector<std::wstring> workflows;
    auto now = std::chrono::system_clock::now();
    for (auto &[name, timer] : _timers) {
      auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(
          now - timer.start);
      if (delay.count() > timer.timeout) {
        workflows.push_back(name);
      }
    }
    for (auto &name : workflows) {
      _timers.erase(name);
      on_timer(name);
    }
  }
}
void widget::clear(core::auto_release<graphic> &g) {
  g->set_attr(_style.normal_attr);
  for (auto x = 0; x < _rect.width; x++) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(x, y, L' ');
    }
  }
}

void widget::calculate_pos() {
  _rect.x = _style.offset.x;
  _rect.y = _style.offset.y;
  if (_style.position == style::RELATIVE) {
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
  _rect.width = _style.size.width;
  if (_rect.width == -1) {
    if (_parent) {
      _rect.width = _parent->_rect.width;
    }
  }
  if (_style.xoverflow == style::SCROLL) {
    for (auto &c : _children) {
      if (c->_style.position == style::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.x + rc.width > _fixed_rect.width + _fixed_rect.x) {
          _fixed_rect.width = rc.x + rc.width - _rect.x - _fixed_rect.x;
        }
      }
    }
  }
  if (_style.xoverflow == style::VISIBLE) {
    for (auto &c : _children) {
      if (c->_style.position == style::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.x + rc.width > _rect.width + _rect.x) {
          _rect.width = rc.x + rc.width - _rect.x - _fixed_rect.x;
        }
      }
    }
    _fixed_rect.x = 0;
    _fixed_rect.width = _rect.width;
  }
  if (_style.max_size.width && _rect.width > _style.max_size.width) {
    _rect.width = _style.max_size.width;
  }
  calculate_fixed();
}
void widget::relayout() {
  if (_style.position == style::ABSOLUTE && widget::_root != this) {
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
  _rect.height = _style.size.height;
  if (_rect.height == -1) {
    if (_parent) {
      _rect.height = _parent->_rect.height;
    }
  }
  if (_style.yoverflow == style::SCROLL) {
    for (auto &c : _children) {
      if (c->_style.position == style::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.y + rc.height > _fixed_rect.height + _fixed_rect.y) {
          _fixed_rect.height = rc.y + rc.height - _rect.y - _fixed_rect.y;
        }
      }
    }
  }
  if (_style.yoverflow == style::VISIBLE) {
    for (auto &c : _children) {
      if (c->_style.position == style::RELATIVE) {
        auto rc = c->get_bound_rect();
        if (rc.y + rc.height > _rect.height + _rect.y) {
          _rect.height = rc.y + rc.height - _rect.y - _fixed_rect.y;
        }
      }
    }
    _fixed_rect.y = 0;
    _fixed_rect.height = _rect.height;
  }
  if (_style.max_size.height && _rect.height > _style.max_size.height) {
    _rect.height = _style.max_size.height;
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
  if (_style.xoverflow == style::FIXED) {
    calculate_width();
  }
  if (_style.yoverflow == style::FIXED) {
    calculate_height();
  }
  for (auto &c : _children) {
    c->request_update();
  }
  if (_style.xoverflow == style::SCROLL || _style.xoverflow == style::VISIBLE) {
    calculate_width();
  }
  if (_style.yoverflow == style::SCROLL || _style.yoverflow == style::VISIBLE) {
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
      next_active();
      return true;
    }
    if (key.name == "<s-tab>") {
      last_active();
      return true;
    }
  }
  return false;
}
void widget::draw_border(core::auto_release<graphic> &g) {
  g->set_attr(_style.border_attr);
  if (_style.border_left) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(-1, y, L'│');
    }
  }
  if (_style.border_right) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(_rect.width, y, L'│');
    }
  }
  if (_style.border_top) {
    for (auto x = 0; x < _rect.width; x++) {
      g->draw(x, -1, L'─');
    }
  }
  if (_style.border_bottom) {
    for (auto x = 0; x < _rect.width; x++) {
      g->draw(x, _rect.height, L'─');
    }
  }

  if (_style.border_left && _style.border_top) {
    g->draw(-1, -1, L'┌');
  }
  if (_style.border_right && _style.border_top) {
    g->draw(_rect.width, -1, L'┐');
  }
  if (_style.border_left && _style.border_bottom) {
    g->draw(-1, _rect.height, L'└');
  }
  if (_style.border_right && _style.border_bottom) {
    g->draw(_rect.width, _rect.height, L'┘');
  }

  if (_style.border_left && !_style.border_top) {
    g->draw(-1, -1, L'│');
  }
  if (!_style.border_left && _style.border_top) {
    g->draw(-1, -1, L'─');
  }

  if (_style.border_right && !_style.border_top) {
    g->draw(_rect.width, -1, L'│');
  }
  if (!_style.border_right && _style.border_top) {
    g->draw(_rect.width, -1, L'─');
  }

  if (_style.border_left && !_style.border_bottom) {
    g->draw(-1, _rect.height - 1, L'│');
  }
  if (!_style.border_left && _style.border_bottom) {
    g->draw(-1, _rect.height, L'─');
  }

  if (_style.border_right && !_style.border_bottom) {
    g->draw(_rect.width, _rect.height, L'│');
  }
  if (!_style.border_right && _style.border_bottom) {
    g->draw(_rect.width, _rect.height, L'─');
  }
}
void widget::on_update() {}
util::rect widget::get_bound_rect() {
  calculate_pos();
  calculate_width();
  calculate_height();
  util::rect rc = _rect;
  if (_style.border_left) {
    rc.x--;
    rc.width++;
  }
  if (_style.border_right) {
    rc.width++;
  }
  if (_style.border_top) {
    rc.y--;
    rc.height++;
  }
  if (_style.border_bottom) {
    rc.height++;
  }
  return rc;
}
style &widget::get_attribute() { return _style; }
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
  g->set_attr(_style.scroll_attr);
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
  if (!_children.empty()) {
    if (_active_widget) {
      _active_widget->on_active();
    }
  }
  _is_active = true;
  request_update();
  emit("focus");
}
void widget::on_dective() {
  if (!_children.empty()) {
    if (_active_widget) {
      _active_widget->on_dective();
    }
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
  std::vector<core::auto_release<widget>> selectables;
  for (auto &c : _children) {
    if (c->_style.selectable) {
      selectables.push_back(c);
    }
  }
  if (selectables.empty()) {
    return false;
  }
  if (_active_widget) {
    auto it = std::find(selectables.begin(), selectables.end(), _active_widget);
    it++;
    if (it == selectables.end()) {
      set_active(nullptr);
      if (_parent) {
        return _parent->next_active();
      } else {
        widget *w = selectables.begin()->get();
        if (!w->_children.empty() && !w->_active_widget) {
          w->next_active();
        }
        set_active(selectables.begin()->get());
      }
      return false;
    } else {
      widget *w = it->get();
      if (!w->_children.empty() && !w->_active_widget) {
        w->next_active();
      }
      set_active(it->get());
      return true;
    }
  } else {
    auto w = selectables.begin()->get();
    if (!w->_children.empty() && !w->_active_widget) {
      w->next_active();
    }
    set_active(selectables.begin()->get());
    return true;
  }
  return false;
}
bool widget::last_active() {
  std::vector<core::auto_release<widget>> selectables;
  for (auto &c : _children) {
    if (c->_style.selectable) {
      selectables.push_back(c);
    }
  }
  if (selectables.empty()) {
    return false;
  }
  if (_active_widget) {
    auto it =
        std::find(selectables.rbegin(), selectables.rend(), _active_widget);
    it++;
    if (it == selectables.rend()) {
      set_active(nullptr);
      if (_parent) {
        return _parent->last_active();
      } else {
        widget *w = selectables.rbegin()->get();
        if (!w->_children.empty() && !w->_active_widget) {
          w->last_active();
        }
        set_active(selectables.rbegin()->get());
      }
      return false;
    } else {
      widget *w = it->get();
      if (!w->_children.empty() && !w->_active_widget) {
        w->last_active();
      }
      set_active(w);
      return true;
    }
  } else {
    auto w = selectables.rbegin()->get();
    if (!w->_children.empty() && !w->_active_widget) {
      w->last_active();
    }
    set_active(w);
    return true;
  }
  return false;
}
bool widget::is_active() { return _is_active; }
bool widget::process_input(const util::key &key) { return on_input(key); }
bool widget::set_timer(const std::wstring &name, const uint64_t &timeout) {
  if (_timers.contains(name)) {
    return false;
  }
  timer_t timer = {std::chrono::system_clock::now(), timeout};
  _timers[name] = timer;
  return true;
}
void widget::clear_timer(const std::wstring &name) { _timers.erase(name); }
void widget::on_timer(const std::wstring &name) {}