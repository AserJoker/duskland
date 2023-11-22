#pragma once
#include "util/include/point.hpp"
#include "util/include/size.hpp"
#include <curses.h>
#include <string>
#ifdef border
#undef border
#endif
namespace duskland::tui {
struct style {
  enum { RELATIVE, ABSOLUTE } position = RELATIVE;
  util::point offset = {0, 0};
  util::size size = {0, 0};
  util::size max_size = {0, 0};
  bool border_left = false, border_right = false, border_top = false,
       border_bottom = false;
  enum { VISIBLE, SCROLL, FIXED } xoverflow = VISIBLE, yoverflow = VISIBLE;
  bool selectable = true;
  std::string normal_attr = "normal";
  std::string active_attr = "active";
  std::string scroll_attr = "scroll";
  std::string border_attr = "border";
};
} // namespace duskland::tui