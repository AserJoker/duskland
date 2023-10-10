#pragma once
#include "widget.hpp"
#include "window.hpp"
namespace duskland::tui {
class widget_base : public widget {
public:
  widget_base(const std::string &name);
  virtual void render(const core::auto_release<window> &win);
};
}; // namespace duskland::tui