#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_container : public widget {
private:
  std::vector<core::auto_release<widget>> _widgets;
  widget *_active_widget;

protected:
  const std::vector<core::auto_release<widget>> &get_widgets() const;
  std::vector<core::auto_release<widget>> &get_widgets();
  virtual util::size get_bound_size() = 0;

public:
  widget_container(const std::string &name);
  core::auto_release<widget> get_active_widget();
  void set_active_widget(const core::auto_release<widget> &active);
  void add_widget(const core::auto_release<widget> &widget);
  void remove_widget(const core::auto_release<widget> &widget);
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
  virtual void on_active() override;
  virtual void on_dective() override;
  void next_active();
};
} // namespace duskland::tui
