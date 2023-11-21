#pragma once
#include "widget.hpp"
namespace duskland::tui {
class layout_vertical : public widget {
protected:
  void on_update() override;

public:
  layout_vertical();
};
} // namespace duskland::tui