
#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_input : public widget {
private:
  std::wstring _value;

  bool _is_reading;
  int32_t _cursor;
  uint32_t _max_length;

private:
  void add_char(const wchar_t& ch);
public:
  widget_input(const std::string &name, const uint32_t &length);
  void render(const core::auto_release<window> &win) override;
  bool on_command(const core::auto_release<window> &win,
                  const util::command &cmd) override;
};
} // namespace duskland::tui