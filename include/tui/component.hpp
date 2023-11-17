#pragma once
#include "widget.hpp"
namespace duskland::tui {
template <class T> class component : public widget {
private:
  std::wstring _name;
  T _value;

public:
  component(const std::wstring &name) : _name(name) {}
  void on_change() { emit("change"); };
  void set_value(const T &val) {
    _value = val;
    on_change();
  }
  const T &get_value() const { return _value; }
  const std::wstring &get_name() const { return _name; }
};
} // namespace duskland::tui