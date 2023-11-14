#pragma once
#include "widget.hpp"
namespace duskland::tui {
class document : public widget {
protected:
  bool on_input(const util::key &key) override;
  void on_render(core::auto_release<graphic> &g) override;

public:
  document();
};
} // namespace duskland::tui