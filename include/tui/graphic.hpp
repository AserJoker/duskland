﻿#pragma once
#include "core/object.hpp"
#include "util/position.hpp"
#include "util/rect.hpp"
#include <curses.h>
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
  void set_attr(chtype attr);
  void draw(int32_t x, int32_t y, wchar_t c);
  void draw_abstruct(int32_t x, int32_t y, wchar_t c);
  void draw(int32_t x, int32_t y, const std::wstring &str);
  void set_view_port(const util::rect &rc);
  void set_position(const util::position &rc);
  const util::position& get_position();
  bool present();
};
} // namespace duskland::tui