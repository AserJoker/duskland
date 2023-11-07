#include "tui/widget.hpp"
#include "util/injector.hpp"
using namespace duskland::tui;
using namespace duskland;
widget::widget()
    : _rect({0, 0, 0, 0}), _offset({0, 0}), _is_selectable(false),
      _is_hidden_overflow(false), _is_active(false), _is_request(false),
      _border({false, false, false, false}), _parent(nullptr),
      _is_visible(true), _is_changed(true), _active_widget(nullptr) {}
const util::rect &widget::get_rect() const { return _rect; }
const util::rect widget::get_content_rect() const {
  util::rect rc = _rect;
  if (_border.left) {
    rc.x += 1;
    rc.width -= 1;
  }
  if (_border.right) {
    rc.width -= 1;
  }
  if (_border.top) {
    rc.y += 1;
    rc.height -= 1;
  }
  if (_border.bottom) {
    rc.height -= 1;
  }
  return rc;
}
void widget::render(const core::auto_release<graphic> &g) {
  if (!_parent) {
    g->set_position({0, 0});
    g->set_view_port({0, 0, 0, 0});
  }
  if (_is_changed) {
    this->clear(g);
    chtype attr = COLOR_PAIR(COLOR_PAIR_INDEX(COLOR_WHITE, COLOR_BLACK));
    if (!_is_active) {
      attr |= WA_DIM;
    }
    g->set_attr(attr);
    this->draw_border(g);
    this->on_render(g);
    _is_changed = false;
  }
  if (_is_hidden_overflow) {
    g->set_view_port(get_content_rect());
  }
  auto pos = g->get_position();
  for (auto &c : _children) {
    auto &crc = get_content_rect();
    g->set_position({crc.x + _offset.x + pos.x, crc.y + _offset.y + pos.y});
    c->render(g);
  }
  if (_is_hidden_overflow) {
    g->set_view_port({0, 0, 0, 0});
  }
}
void widget::clear(const core::auto_release<graphic> &g) {
  for (auto x = 0; x < _rect.width; x++) {
    for (auto y = 0; y < _rect.height; y++) {
      g->draw(_rect.x + x, _rect.y + y, L' ');
    }
  }
}
bool widget::is_selectable() { return _is_selectable; };
void widget::set_selectable(bool selectable) { _is_selectable = selectable; };
bool widget::is_active() { return _is_active; }
void widget::draw_border(const core::auto_release<graphic> &g) {
  auto &rc = get_rect();
  if (_border.left && _border.top) {
    g->draw(rc.x, rc.y, L'┌');
  } else if (!_border.left && _border.top) {
    g->draw(rc.x, rc.y, L'─');
  } else if (_border.left && !_border.top) {
    g->draw(rc.x, rc.y, L'│');
  }
  if (_border.right && _border.top) {
    g->draw(rc.x + rc.width - 1, rc.y, L'┐');
  } else if (!_border.right && _border.top) {
    g->draw(rc.x + rc.width - 1, rc.y, L'─');
  } else if (_border.right && !_border.top) {
    g->draw(rc.x + rc.width - 1, rc.y, L'│');
  }
  if (_border.left && _border.bottom) {
    g->draw(rc.x, rc.y + rc.height - 1, L'└');
  } else if (!_border.left && _border.bottom) {
    g->draw(rc.x, rc.y + rc.height - 1, L'─');
  } else if (_border.left && !_border.bottom) {
    g->draw(rc.x, rc.y + rc.height - 1, L'│');
  }
  if (_border.right && _border.bottom) {
    g->draw(rc.x + rc.width - 1, rc.y + rc.height - 1, L'┘');
  } else if (!_border.right && _border.bottom) {
    g->draw(rc.x + rc.width - 1, rc.y + rc.height - 1, L'─');
  } else if (_border.right && !_border.bottom) {
    g->draw(rc.x + rc.width - 1, rc.y + rc.height - 1, L'│');
  }
  if (rc.width) {
    for (auto x = 1; x < rc.width - 1; x++) {
      if (_border.top) {
        g->draw(rc.x + x, rc.y, L'─');
      }
      if (_border.bottom) {
        g->draw(rc.x + x, rc.y + rc.height - 1, L'─');
      }
    }
  }
  if (rc.height) {
    for (auto y = 1; y < rc.height - 1; y++) {
      if (_border.left) {
        g->draw(rc.x, rc.y + y, L'│');
      }
      if (_border.right) {
        g->draw(rc.x + rc.width - 1, rc.y + y, L'│');
      }
    }
  }
}
void widget::set_border(const util::border_info &border) {
  _border = border;
  request_update();
}
widget *widget::get_active_widget() { return _active_widget; }
void widget::on_render(const core::auto_release<graphic> &g) {}
bool widget::on_input(const util::key &key) {
  if (_active_widget) {
    if (_active_widget->on_input(key)) {
      return true;
    }
  }
  if (key.name == "<tab>") {
    if (next_active()) {
      return true;
    }
  }
  return false;
}
void widget::on_active() {
  if (_children.empty()) {
    emit("focus");
  } else {
    if (_active_widget) {
      _active_widget->on_active();
    } else {
      next_active();
    }
  }
  _is_active = true;
}
void widget::on_dective() {
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _is_active = false;
}
void widget::request_update() {
  if (_is_request) {
    return;
  }
  _is_request = true;
  _is_changed = true;
  on_update();
  _is_request = false;
  for (auto &c : _children) {
    c->request_update();
  }
}

void widget::set_content_rect(const util::rect &rect) {
  _rect = rect;
  if (_border.left) {
    _rect.width++;
  }
  if (_border.right) {
    _rect.width++;
  }
  if (_border.top) {
    _rect.height++;
  }
  if (_border.bottom) {
    _rect.height++;
  }
  request_update();
}
std::vector<core::auto_release<widget>> &widget::get_children() {
  return _children;
}
widget *widget::get_parent() { return _parent; }
const util::border_info &widget::get_border() const { return _border; }
void widget::add_child(const core::auto_release<widget> &widget) {
  for (auto &c : _children) {
    if (c == widget) {
      return;
    }
  }
  _children.push_back(widget);
  widget->_parent = this;
  request_update();
}
void widget::remove_child(const core::auto_release<widget> &widget) {
  for (auto it = _children.begin(); it != _children.end(); it++) {
    if (*it == widget) {
      if (widget == _active_widget) {
        next_active();
      }
      _children.erase(it);
      break;
    }
  }
  request_update();
}
bool widget::next_active() {
  if (_active_widget == nullptr) {
    for (auto &c : _children) {
      if (c->is_visible() && c->is_selectable()) {
        set_active_widget(c);
        return true;
      }
    }
  } else {
    auto it = std::find(_children.begin(), _children.end(), _active_widget);
    auto next = it + 1;
    while (next != _children.end()) {
      if ((*next)->is_visible() && (*next)->is_selectable()) {
        set_active_widget(*next);
        return true;
      }
      next++;
    }
    set_active_widget(nullptr);
  }
  return false;
}
void widget::set_active_widget(const core::auto_release<widget> &widget) {
  if (_active_widget != widget.get()) {
    if (_active_widget) {
      _active_widget->on_dective();
    }
    _active_widget = widget.get();
    if (_active_widget) {
      _active_widget->on_active();
    }
    request_update();
  }
}
void widget::set_rect(const util::rect &rc) {
  _rect = rc;
  if (_rect.width < 10) {
    _rect.width = 10;
  }
  request_update();
  if (_parent) {
    _parent->request_update();
  }
}
void widget::set_visible(bool visible) {
  _is_visible = visible;
  if (_is_active) {
    if (_parent) {
      _parent->next_active();
    }
  }
  request_update();
}
const bool &widget::is_visible() const { return _is_visible; }
void widget::on_update() {}

void widget::emit(const std::string &event) { on_event(event, this); }
void widget::on_event(const std::string &event, widget *emitter) {
  if (_parent) {
    _parent->on_event(event, emitter);
  }
}
const util::position &widget::get_offset() const { return _offset; }
void widget::set_offset(const util::position &pos) {
  _offset = pos;
  request_update();
}
util::rect widget::get_absolute_rect() {
  if (_parent) {
    auto &rc = get_rect();
    auto parc = _parent->get_absolute_rect();
    auto crc = _parent->get_content_rect();
    auto &prc = _parent->get_rect();
    return {(crc.x - prc.x) + parc.x + rc.x, (crc.y - prc.y) + parc.y + rc.y,
            rc.width, rc.height};
  } else {
    return get_rect();
  }
}
void widget::set_hidden_overflow(bool hidden_overflow) {
  _is_hidden_overflow = hidden_overflow;
}
bool widget::is_hidden_overflow() { return _is_hidden_overflow; }