#pragma once
#include "component.hpp"
#include "option.hpp"
namespace duskland::tui {
class list_select : public component<std::wstring> {
private:
  std::vector<util::option> _options;
  bool _is_selecting;
  option *_current;

public:
  list_select(const std::wstring &name, const std::vector<util::option> &opt);
  bool on_input(const util::key &key) override;
  void on_event(const std::string &event, widget *emitter) override;
  void set_options(const std::vector<util::option>& opt);
};
} // namespace duskland::tui