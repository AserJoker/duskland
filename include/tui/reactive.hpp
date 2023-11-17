#pragma once
#include "component.hpp"
#include <functional>
namespace duskland::tui {
using setter_t = std::function<void(widget *)>;
template <class T> class rective : public component<T> {
private:
  std::map<std::wstring, setter_t> _setters;

public:
  rective(const std::wstring &name) : component<T>(name) {}
  template <class K> void define(const std::wstring &name, K T::*handle) {
    setter_t setter = [this](widget *w) -> void {
      auto val = this->get_value();
      (val->*T::handle) = ((component<K> *)w)->get_value();
      this->set_value(val);
    };
    _setters[name] = setter;
  }
  void on_event(const std::string &event, widget *emitter) {
    if (event == "change") {
      auto w = (base_component *)emitter;
      auto &name = w->get_name();
      if (_setters.contains(name)) {
        _setters.at(name)(w);
        return;
      }
    }
    widget::on_event(event, emitter);
  }
};
} // namespace duskland::tui