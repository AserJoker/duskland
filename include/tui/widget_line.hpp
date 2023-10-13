﻿#pragma once
#include "widget_container.hpp"
namespace duskland::tui {
class widget_line : public widget_container {
protected:
  util::size get_bound_size() override;

public:
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  widget_line(const std::string &name);
};
}; // namespace duskland::tui