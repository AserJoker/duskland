#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_container : public widget {
private:
  std::vector<core::auto_release<widget>> _children;
  widget *_active_widget;

protected:
  std::vector<core::auto_release<widget>> &get_children();

public:
  widget_container(const std::string &name);
  void add_widget(const core::auto_release<widget> &widget);
  void remove_widget(const core::auto_release<widget> &widget);
  bool on_command(const core::auto_release<window> &win,
                  const util::command &cmd) override;
  void on_active() override;
  void on_dective() override;
  void set_active_widget(const core::auto_release<widget> &widget);
  bool next_active();

  virtual void on_children_change();
};
} // namespace duskland::tui