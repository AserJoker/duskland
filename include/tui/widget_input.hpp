#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_input : public widget {
private:
  uint32_t _max_length;
  std::wstring _input;
  int _mode;
  int _cursor;
  void on_input(const wchar_t &ch);
  void on_backspace();
  wint_t decode_input(const std::vector<wint_t> &codes);

public:
  widget_input(const std::string &name, const uint32_t &max_length);
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  bool on_command(wint_t cmd,
                  const core::auto_release<widget_base> &emitter) override;
  const std::wstring &get_input() const;
};
} // namespace duskland::tui