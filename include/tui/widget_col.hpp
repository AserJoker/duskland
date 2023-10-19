﻿#pragma once
#include "widget_container.hpp"
namespace duskland::tui {
class widget_col : public widget_container {
public:
  widget_col(const std::string &name);
  void render(const core::auto_release<window> &win) override;
  void on_children_change() override;
};
} // namespace duskland::tui