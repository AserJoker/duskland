#pragma once
#include "widget.hpp"
namespace duskland::tui {
class layout_horizontal : public widget {
public:
  layout_horizontal();
  void on_update() override;
};
} // namespace duskland::tui