#pragma once
#include "core/object.hpp"
#include "util/position.hpp"
#include <curses.h>
#include <string>
namespace duskland::tui {
class graphic : public core::object {
private:
  bool _need_update;
  util::position _position;

public:
  graphic();
  ~graphic() override;
  void set_attr(chtype attr);
  void draw(int32_t x, int32_t y, wchar_t c);
  void draw(int32_t x, int32_t y, const std::wstring &str);
  bool present();
};
} // namespace duskland::tui