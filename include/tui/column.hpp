﻿#pragma once
#include "widget.hpp"
namespace duskland::tui {
class column : public widget {
public:
  column();
  void on_update() override;
  void on_active() override;
};
} // namespace duskland::tui