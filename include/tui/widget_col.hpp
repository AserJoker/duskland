#pragma once
#include "widget_container.hpp"
namespace duskland::tui {
class widget_col : public widget_container {
public:
  void render(const core::auto_release<window> &win);
  widget_col(const std::string &name);
};
} // namespace duskland::tui