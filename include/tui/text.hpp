#pragma once
#include "widget.hpp"
namespace duskland::tui {
class text : public widget {
private:
  std::vector<std::wstring> _lines;

public:
  text(const std::wstring &text = L"");
  void set_text(const std::wstring &text);
  std::wstring get_text() const;
  void on_render(core::auto_release<graphic> &g) override;
  void on_update() override;
};
} // namespace duskland::tui