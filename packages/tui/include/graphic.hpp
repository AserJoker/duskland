﻿#pragma once
#include "brush.hpp"
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "util/include/point.hpp"
#include "util/include/rect.hpp"
#include "util/include/tile.hpp"
#include <string>
namespace duskland::tui {
class graphic : public core::object {
private:
  bool _need_update;
  util::point _position;
  util::rect _viewport;
  core::auto_release<brush> _brush;
  bool _ready;

public:
  graphic();
  ~graphic() override;
  void set_attr(const std::string &name);
  void draw(int32_t x, int32_t y, wchar_t c);
  void draw(int32_t x, int32_t y, const std::wstring &str);
  void draw(int32_t x, int32_t y, const util::tile& tile);
  void set_viewport(const util::rect &rc);
  const util::rect &get_viewport() const;
  void set_position(const util::point &rc);
  const util::point &get_position();
  bool present();
  void initialize(const core::auto_release<brush> &color);
  void uninitialize();
  core::auto_release<brush> get_brush();
};
} // namespace duskland::tui