#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_text : public widget {
private:
  std::wstring _text;

public:
  widget_text(const std::string &name, const std::wstring &text);
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  void set_text(const std::wstring &text);
  const std::wstring &get_text();
};
}; // namespace duskland::tui