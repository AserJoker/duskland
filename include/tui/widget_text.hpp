#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_text : public widget {
private:
  std::vector<std::wstring> _text;
  int32_t _max_length;

public:
  widget_text(const std::string &name, const std::wstring &text,
              const int32_t &max_length = 0);
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  void set_text(const std::wstring &text);
  const std::wstring get_text();
};
}; // namespace duskland::tui