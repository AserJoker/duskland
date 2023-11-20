#pragma once
#include "tui/input.hpp"
#include "widget.hpp"
#include <codecvt>
namespace duskland::tui {
class document : public widget {
protected:
  bool on_input(const util::key &key) override;

  std::wstring name;
  std::wstring val;

public:
  document();
};
} // namespace duskland::tui