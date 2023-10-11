#pragma once
#include "widget_form.hpp"
#include "window.hpp"
namespace duskland::tui {
class window_form : public window {
private:
  core::auto_release<widget_form> _form;

public:
  window_form(const util::rect &rc, const std::string &name);
  core::auto_release<widget_form> get_form();
  void on_update() override;
  void on_active() override;
  void on_dective() override;
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
};
}; // namespace duskland::tui