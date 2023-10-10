#pragma once
#include "widget_base.hpp"
#include "window.hpp"
namespace duskland::tui {
class window_form : public window {
private:
  std::vector<core::auto_release<widget_base>> _widgets;
  widget *_active_widget;
  void draw_scroll();

protected:
  std::vector<core::auto_release<widget_base>> &get_widgets();

public:
  void on_command(int ch) override;
  void on_active() override;
  void on_dective() override;
  void on_update() override;
  window_form(const util::rect &rc, const std::string &name);
  void set_active_widget(widget_base *widget);
};
}; // namespace duskland::tui