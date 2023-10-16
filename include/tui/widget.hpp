#pragma once
#include "system_tui.hpp"
#include "util/position.hpp"
#include "widget_base.hpp"
#include "window.hpp"
namespace duskland::tui {
class widget : public widget_base {
protected:
  core::auto_release<util::config> _config;
  core::auto_release<system_tui> _tui;

public:
  widget(const std::string &name);
  virtual void render(const core::auto_release<window> &win,
                      const util::position &pos);
  bool on_command(wint_t cmd,
                  const core::auto_release<widget_base> &emitter) override;
  void on_active() override;
};
} // namespace duskland::tui