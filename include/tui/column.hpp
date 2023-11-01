#pragma once
#include "widget.hpp"
namespace duskland::tui {
class column : public widget {
public:
  column();
  void on_render(const core::auto_release<graphic> &g) override;
  void on_update() override;
  void on_active() override;
};
} // namespace duskland::tui