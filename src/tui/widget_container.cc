#include "tui/widget_container.hpp"
using namespace duskland::tui;
using namespace duskland;

widget_container::widget_container(const std::string &name)
    : widget(name), _active_widget(nullptr) {}
void widget_container::add_widget(const core::auto_release<widget> &widget) {
  _children.push_back(widget);
  auto &crc = get_rect();
  auto &rc = widget->get_rect();
  set_rect({0, 0, crc.width < rc.width ? rc.width : crc.width,
            crc.height + rc.height});
}
void widget_container::remove_widget(const core::auto_release<widget> &widget) {
  for (auto it = _children.begin(); it != _children.end(); it++) {
    if (*it == widget) {
      if (widget == _active_widget) {
        next_active();
      }
      if (widget == _active_widget) {
        _active_widget = nullptr;
      }
      _children.erase(it);
      break;
    }
  }
}
bool widget_container::on_command(const core::auto_release<window> &win,
                                  const wint_t &cmd) {
  if (_active_widget) {
    if (_active_widget->on_command(win, cmd)) {
      return true;
    }
  }
  if (cmd == _injector->keymap("key.next")) {
    if (next_active()) {
      win->render();
      return true;
    } else {
      win->render();
      return false;
    }
  }
  return widget::on_command(win, cmd);
}
void widget_container::on_active() {
  if (!_active_widget) {
    next_active();
  }
  if (_active_widget) {
    _active_widget->on_active();
  }
}
void widget_container::on_dective() {
  if (_active_widget) {
    _active_widget->on_dective();
  }
}
void widget_container::set_active_widget(
    const core::auto_release<widget> &widget) {
  if (widget == _active_widget) {
    return;
  }
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _active_widget = widget.get();
  if (_active_widget) {
    _active_widget->on_active();
  }
}
bool widget_container::next_active() {
  if (!_active_widget) {
    for (auto it = _children.begin(); it != _children.end(); it++) {
      if ((*it)->get_tab_index() != -1) {
        set_active_widget(*it);
        return true;
      }
    }
    return false;
  } else {
    for (auto it = _children.begin(); it != _children.end(); it++) {
      if (*it == _active_widget) {
        auto next = it + 1;
        while (next != it) {
          if (next == _children.end()) {
            set_active_widget(_children.begin()->get());
            return false;
          }
          if ((*next)->get_tab_index() != -1) {
            break;
          }
          next++;
        }
        if (next != it) {
          set_active_widget(*next);
        }
        return true;
      }
    }
  }
  return false;
}
std::vector<core::auto_release<widget>> &widget_container::get_children() {
  return _children;
}