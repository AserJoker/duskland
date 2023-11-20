#pragma once
#include "widget.hpp"
namespace duskland::tui {
class layout_vertical : public widget {
public:
  layout_vertical();
  void on_update() override;
};
} // namespace duskland::tui