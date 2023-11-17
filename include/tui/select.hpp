#pragma once
#include "component.hpp"
#include "util/option.hpp"
namespace duskland::tui {
class select : public component<std::wstring> {
private:
  std::vector<util::option> _options;

public:
  select(const std::wstring &name, const std::vector<util::option> &options);
  bool on_input(const util::key &key) override;
  void on_render(core::auto_release<graphic> &g) override;
};
} // namespace duskland::tui