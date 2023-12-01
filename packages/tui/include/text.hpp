#pragma once
#include "core/include/auto_release.hpp"
#include "widget.hpp"
#include <string>
namespace duskland::tui {
class text : public widget {
private:
  std::wstring _text;

protected:
  void render(core::auto_release<graphic> &g) override;

public:
  text(const std::wstring &text);
  void set_text(const std::wstring &text);
  const std::wstring &get_text() const;
};
} // namespace duskland::tui