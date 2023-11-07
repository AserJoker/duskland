#pragma once
#include "core/object.hpp"
#include "util/position.hpp"
#include "util/rect.hpp"
#include <string>
namespace duskland::tui {
class graphic : public core::object {
private:
  bool _need_update;
  util::position _position;
  util::rect _viewport;

public:
  graphic();
  ~graphic() override;
  void set_attr(uint32_t attr);
  void draw(int32_t x, int32_t y, wchar_t c);
  void draw_abstruct(int32_t x, int32_t y, wchar_t c);
  void draw(int32_t x, int32_t y, const std::wstring &str);
  void set_viewport(const util::rect &rc);
  const util::rect &get_viewport() const;
  void set_position(const util::position &rc);
  const util::position &get_position();
  bool present();
  void initialize();
  void uninitialize();
};
} // namespace duskland::tui