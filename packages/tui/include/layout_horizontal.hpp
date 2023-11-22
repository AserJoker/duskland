#pragma once
#include "widget.hpp"
namespace duskland::tui {
class layout_horizontal : public widget {
protected:
  void on_update() override;

public:
  layout_horizontal();
};
} // namespace duskland::tui