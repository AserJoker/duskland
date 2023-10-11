#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/rect.hpp"
#include <string>
#include <vector>
namespace duskland::tui {
class widget_base : public core::object {
private:
  bool _is_active;
  std::string _name;
  util::rect _rect;
  int _select_index;

public:
  virtual void on_active();
  virtual void on_dective();
  virtual bool on_command(int cmd,
                          const core::auto_release<widget_base> &emitter);
  virtual void on_update();
  const util::rect &get_rect() const;
  util::rect &get_rect();
  const std::string &get_name() const;
  void set_name(const std::string &name);
  bool is_active();
  widget_base(const std::string &name);
  int get_select_index();
  void set_select_index(int value);
};
}; // namespace duskland::tui