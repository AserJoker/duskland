#pragma once
#include "widget.hpp"
#include "window.hpp"
namespace duskland::tui {
class window_widget : public window {
private:
  core::auto_release<widget> _root;

public:
  window_widget(const util::rect &rc, const std::string &name);
  core::auto_release<widget> &get_root();
  void on_update() override;
  void on_active() override;
  void on_dective() override;
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
};
}; // namespace duskland::tui