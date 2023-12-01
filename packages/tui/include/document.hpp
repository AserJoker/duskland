#pragma once
#include "core/include/auto_release.hpp"
#include "tui/include/graphic.hpp"
#include "util/include/key.hpp"
#include "widget.hpp"
namespace duskland::tui {
class document : public widget {
protected:
  bool process(const util::key &key) override;
  void render(core::auto_release<graphic> &g) override;

public:
  document();
};
} // namespace duskland::tui