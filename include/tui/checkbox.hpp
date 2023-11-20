#pragma once
#include "component.hpp"
namespace duskland::tui {
class checkbox : public component<bool> {
private:
  std::wstring _display_name;

public:
  checkbox(const std::wstring &name, const std::wstring &display_name = L"");
  void on_render(core::auto_release<graphic> &g) override;
  bool on_input(const util::key &key) override;
};
} // namespace duskland::tui