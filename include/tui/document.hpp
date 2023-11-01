#pragma once
#include "widget.hpp"
namespace duskland::tui {
class document : public widget {
public:
  document();
  bool on_input(const util::key &key);
};
} // namespace duskland::tui