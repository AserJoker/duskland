#pragma once
#include "widget.hpp"
namespace duskland::tui {
class line : public widget {
public:
  line();
  void on_update() override;
  void on_active() override;
};
} // namespace duskland::tui