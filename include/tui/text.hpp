#pragma once
#include "widget.hpp"
namespace duskland::tui {
class text : public widget {
private:
  std::wstring _text;

public:
  text(const std::wstring &text = L"");
  void on_render(const core::auto_release<graphic> &g);
  void set_text(const std::wstring &text);
};
} // namespace duskland::tui