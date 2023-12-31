﻿#pragma once
#include "component.hpp"
#include "widget.hpp"
namespace duskland::tui {
class input : public component<std::wstring> {
private:
  bool _is_input;
  int _cursor;
  int32_t _max_length;
  std::wstring _value;
  bool _show_cursor;

private:
  void set_cursor(int32_t cursor);
  void on_input(wchar_t ch);
  void on_backspace();

protected:
  bool on_input(const util::key &key) override;
  void on_render(core::auto_release<graphic> &g) override;
  void on_timer(const std::wstring &name) override;

public:
  input(const std::wstring &name, int32_t max_length);
};
} // namespace duskland::tui