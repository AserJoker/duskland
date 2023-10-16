#include "tui/widget_container.hpp"
using namespace duskland::tui;
using namespace duskland;

widget_container::widget_container(const std::string &name)
    : widget(name), _active_widget(nullptr) {
  set_select_index(-1);
};
std::vector<core::auto_release<widget>> &widget_container::get_widgets() {
  return _widgets;
}
const std::vector<core::auto_release<widget>> &
widget_container::get_widgets() const {
  return _widgets;
}
core::auto_release<widget> widget_container::get_active_widget() {
  return _active_widget;
}
void widget_container::set_active_widget(
    const core::auto_release<widget> &active) {
  if (active == _active_widget) {
    return;
  }
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _active_widget = active.get();
  if (active != nullptr) {
    _active_widget->on_active();
  }
}
void widget_container::add_widget(const core::auto_release<widget> &widget) {
  _widgets.push_back(widget);
  set_select_index(0);
  auto &rc = get_rect();
  auto size = get_bound_size();
  rc.width = size.width;
  rc.height = size.height;
}
void widget_container::remove_widget(const core::auto_release<widget> &widget) {
  for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
    if (*it == widget) {
      auto next = it + 1;
      if (next == _widgets.end()) {
        set_active_widget(nullptr);
      } else {
        set_active_widget(*next);
      }
      _widgets.erase(it);
      break;
    }
  }
  if (_widgets.empty()) {
    set_select_index(-1);
  }
  auto &rc = get_rect();
  auto size = get_bound_size();
  rc.width = size.width;
  rc.height = size.height;
}
bool widget_container::on_command(
    wint_t cmd, const core::auto_release<widget_base> &emitter) {
  if (_active_widget) {
    if (_active_widget->on_command(cmd, emitter)) {
      return true;
    }
  }
  if (cmd == _injector->keymap("key.next")) {
    next_active();
    if (!_active_widget) {
      return false;
    }
    return true;
  }
  if (cmd == _injector->keymap("key.select")) {
    return false;
  }
  return widget::on_command(cmd, emitter);
}
void widget_container::on_active() {
  auto it = _widgets.begin();
  while (it != _widgets.end()) {
    if (it->get()->get_select_index() != -1) {
      break;
    }
    it++;
  }
  if (it != _widgets.end()) {
    set_active_widget(*it);
  }
}
void widget_container::on_dective() {
  if (_active_widget) {
    _active_widget->on_dective();
    _active_widget = nullptr;
  }
}
void widget_container::next_active() {
  if (_active_widget == nullptr) {
    if (_widgets.empty()) {
      return;
    }
    auto it = _widgets.begin();
    while (it != _widgets.end()) {
      if (it->get()->get_select_index() != -1) {
        break;
      }
      it++;
    }
    if (it != _widgets.end()) {
      set_active_widget(*it);
    }
  } else {
    for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
      if (*it == _active_widget) {
        auto next = it + 1;
        while (next != _widgets.end()) {
          if ((*next)->get_select_index() != -1) {
            break;
          }
          next++;
        }
        if (next == _widgets.end()) {
          set_active_widget(nullptr);
        } else {
          set_active_widget(*next);
        }
        break;
      }
    }
  }
}