#pragma once
#include "widget.hpp"
namespace duskland::tui {
class text : public widget {
private:
  std::vector<std::wstring> _lines;
  bool _auto_wrap;

protected:
  void on_render(core::auto_release<graphic> &g) override;
  void on_update() override;

public:
  text(const std::wstring &text = L"");
  void set_text(const std::wstring &text);
  std::wstring get_text() const;
  void set_auto_wrap(const bool &auto_wrap);
  const bool &get_auto_wrap() const;
};
} // namespace duskland::tui