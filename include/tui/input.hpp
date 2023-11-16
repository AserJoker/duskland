#pragma once
#include "widget.hpp"
namespace duskland::tui {
class input : public widget {
private:
  std::wstring _value;
  bool _is_input;
  int _cursor;
  int32_t _max_length;

private:
  void set_cursor(int32_t cursor);
  void on_input(wchar_t ch);
  void on_backspace();

public:
  input(int32_t max_length);
  bool on_input(const util::key &key) override;
  void on_render(core::auto_release<graphic> &g) override;
  void on_update() override;
};
} // namespace duskland::tui