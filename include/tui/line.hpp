#pragma once
#include "widget.hpp"
namespace duskland::tui {
class line : public widget {
public:
  line();
  void on_update() override;
};
} // namespace duskland::tui