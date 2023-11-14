#pragma once
#include "util/point.hpp"
#include "util/size.hpp"
#include <curses.h>
#include <string>
#define COLOR_PAIR_INDEX(fg, bg) (int16_t)(fg * 8 + bg)
namespace duskland::tui {
struct attribute {
  enum { RELATIVE, ABSOLUTE } position = RELATIVE;
  util::point offset = {0, 0};
  util::size size = {0, 0};
  util::size max_size = {0, 0};
  struct {
    bool left = false, right = false, top = false, bottom = false;
    std::string attr = "border";
  } border;
  enum { VISIBLE, SCROLL, FIXED } xoverflow = VISIBLE, yoverflow = VISIBLE;
  bool selectable = true;
  std::string attr = "normal";
  std::string attr_active = "active";
  std::string attr_scroll = "scroll";
};
} // namespace duskland::tui