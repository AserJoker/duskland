#pragma once
#include "widget.hpp"
#include "window.hpp"
namespace duskland::tui {
class window_widget : public window {
private:
  core::auto_release<widget> _root;

protected:
  core::auto_release<widget> &get_root();

public:
  window_widget(const std::string& name);
  void on_initialize() override;
  void on_active() override;
  void on_dective() override;
  bool on_command(const wint_t &cmd) override;
  void on_render() override;
  void on_dispose() override;
};
} // namespace duskland::tui