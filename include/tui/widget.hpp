#pragma once
#include "core/object.hpp"
#include "util/rect.hpp"
#include <string>
#include <vector>
namespace duskland::tui {
class widget : public core::object {
private:
  bool _is_active;
  std::string _name;
  util::rect _rect;
  bool _is_visible;

public:
  virtual void on_active();
  virtual void on_dective();
  virtual void on_command(int cmd);
  virtual void on_update();
  const util::rect &get_rect() const;
  util::rect &get_rect();
  const std::string &get_name() const;
  void set_name(const std::string &name);
  bool is_active();
  widget(const std::string &name);
};
}; // namespace duskland::tui