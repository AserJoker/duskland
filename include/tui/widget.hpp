#pragma once
#include "util/position.hpp"
#include "widget_base.hpp"
#include "window.hpp"
namespace duskland::tui {
class widget : public widget_base {

public:
  widget(const std::string &name);
  virtual void render(const core::auto_release<window> &win,
                      const util::position &pos);
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
};
} // namespace duskland::tui