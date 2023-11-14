#pragma once
#include "widget.hpp"
namespace duskland::tui {
class document : public widget {
protected:
  bool on_input(const util::key &key) override;

public:
  document();
};
} // namespace duskland::tui