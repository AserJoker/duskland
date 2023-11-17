#pragma once
#include "widget.hpp"
namespace duskland::tui {
class base_component : public widget {
private:
  std::wstring _name;

public:
  base_component(const std::wstring &name) : _name(name) {}
  const std::wstring &get_name() const { return _name; }
};
template <class T> class component : public base_component {
private:
  T _value;

public:
  component(const std::wstring &name) : base_component(name) {}
  void on_change() {
    emit("change");
    request_update();
  };
  void set_value(const T &val) {
    _value = val;
    on_change();
  }
  const T &get_value() const { return _value; }
};
} // namespace duskland::tui