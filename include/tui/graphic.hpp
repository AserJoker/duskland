#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/color.hpp"
#include "util/point.hpp"
#include "util/rect.hpp"
#include <string>
namespace duskland::tui {
class graphic : public core::object {
private:
  bool _need_update;
  util::point _position;
  util::rect _viewport;
  core::auto_release<util::color> _colors;

public:
  graphic();
  ~graphic() override;
  void set_attr(const std::string &name);
  void draw(int32_t x, int32_t y, wchar_t c);
  void draw_abstruct(int32_t x, int32_t y, wchar_t c);
  void draw(int32_t x, int32_t y, const std::wstring &str);
  void set_viewport(const util::rect &rc);
  const util::rect &get_viewport() const;
  void set_position(const util::point &rc);
  const util::point &get_position();
  bool present();
  void initialize(const core::auto_release<util::color> &color);
  void uninitialize();
};
} // namespace duskland::tui