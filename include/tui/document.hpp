#pragma once
#include "widget.hpp"
namespace duskland::tui {
class document : public widget {
public:
  document();
  bool on_input(const util::key &key);
  void on_render(const core::auto_release<graphic> &g);
};
} // namespace duskland::tui