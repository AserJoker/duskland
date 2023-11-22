#pragma once
#include "component.hpp"
#include "util/include/option.hpp"
namespace duskland::tui {
class select : public component<std::wstring> {
private:
  std::vector<util::option> _options;

protected:
  bool on_input(const util::key &key) override;
  void on_render(core::auto_release<graphic> &g) override;

public:
  select(const std::wstring &name, const std::vector<util::option> &options);
};
} // namespace duskland::tui