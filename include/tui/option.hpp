#pragma once
#include "util/option.hpp"
#include "widget.hpp"
namespace duskland::tui {
class option : public widget {
private:
  util::option _opt;
  bool _is_selected;

public:
  option(const util::option &opt);
  void on_render(core::auto_release<graphic> &g) override;
  bool on_input(const util::key &key) override;
  void set_selected(bool selected);
  bool is_selected() const;
  const std::wstring &get_value() const;
};
} // namespace duskland::tui
