#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_form : public widget {
private:
  util::size get_bound_size();
  std::vector<core::auto_release<widget>> _widgets;
  widget *_active_widget;
  void fix_rect();

public:
  widget_form(const std::string &name);
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  void add_widget(const core::auto_release<widget> &widget);
  void remove_widget(const core::auto_release<widget> &widget);
  void remove_widget(const std::string& name);
  void on_active() override;
  void on_dective() override;
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
  void set_active_widget(widget *w);
  widget *get_active_widget();
  void next_active();
};
} // namespace duskland::tui