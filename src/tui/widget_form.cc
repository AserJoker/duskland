#include "tui/widget_form.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_form::widget_form(const std::string &name)
    : widget(name), _active_widget(nullptr) {}
void widget_form::render(const core::auto_release<window> &win,
                         const util::position &pos) {
  util::position stream = pos;
  for (auto &w : _widgets) {
    w->render(win, stream);
    stream.y += w->get_rect().height;
  }
}
void widget_form::fix_rect() {
  util::size size = {0, 0};
  for (auto &w : _widgets) {
    auto &rc = w->get_rect();
    if (size.width < rc.width) {
      size.width = rc.width;
    }
    size.height += rc.height;
  }
  auto &rc = get_rect();
  rc.width = size.width;
  rc.height = size.height;
}

void widget_form::add_widget(const core::auto_release<widget> &widget) {
  _widgets.push_back(widget);
  fix_rect();
}
void widget_form::remove_widget(const std::string &name) {
  if (_active_widget && _active_widget->get_name() == name) {
    _active_widget = nullptr;
  }
  for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
    if (it->get()->get_name() == name) {
      _widgets.erase(it);
      break;
    }
  }
  fix_rect();
  if (!_active_widget) {
    next_active();
  }
}
void widget_form::remove_widget(const core::auto_release<widget> &widget) {
  if (widget == _active_widget) {
    _active_widget = nullptr;
  }
  for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
    if (*it == widget) {
      _widgets.erase(it);
      break;
    }
  }
  fix_rect();
  if (!_active_widget) {
    next_active();
  }
}

void widget_form::on_active() {
  if (_active_widget) {
    _active_widget->on_active();
  }
};
void widget_form::on_dective() {
  if (_active_widget) {
    _active_widget->on_dective();
  }
};
bool widget_form::on_command(int cmd,
                             const core::auto_release<widget_base> &emitter) {
  if (_active_widget) {
    if (_active_widget->on_command(cmd, emitter)) {
      return true;
    }
  }
  if (cmd == '\t') {
    next_active();
    core::auto_release<window> win = (window *)emitter.get();
    win->update();
  }
  return widget::on_command(cmd, emitter);
};
void widget_form::set_active_widget(widget *w) {
  if (_active_widget == w) {
    return;
  }
  if (w->get_select_index() == -1) {
    return;
  }
  if (_active_widget) {
    _active_widget->on_dective();
  }
  _active_widget = w;
  if (_active_widget) {
    _active_widget->on_active();
  }
};
void widget_form::next_active() {
  widget *active_one = nullptr;
  for (auto it = _widgets.begin(); it != _widgets.end(); it++) {
    if (*it == _active_widget) {
      auto next = it + 1;
      if (next == _widgets.end()) {
        next = _widgets.begin();
      }
      active_one = next->get();
      break;
    }
    if ((*it)->get_select_index() != -1) {
      active_one = it->get();
    }
  }
  if (active_one) {
    set_active_widget(active_one);
  }
}
widget *widget_form::get_active_widget() { return _active_widget; }