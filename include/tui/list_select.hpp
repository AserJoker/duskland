#pragma once
#include "component.hpp"
#include "util/option.hpp"
namespace duskland::tui {
class list_select : public component<std::wstring> {
private:
  std::vector<util::option> _options;
  std::wstring _current;
  bool _is_selecting;

public:
  list_select(const std::wstring &name, const std::vector<util::option> &opt);
  void on_render(core::auto_release<graphic> &g) override;
  bool on_input(const util::key &key) override;
  void on_active() override;
  void on_dective() override;
};
} // namespace duskland::tui