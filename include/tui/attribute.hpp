#pragma once
#include "util/point.hpp"
#include "util/size.hpp"
#include <string>
namespace duskland::tui {
struct attribute {
  enum { RELATIVE, ABSOLUTE } position = RELATIVE;
  util::point offset = {0, 0};
  util::size size = {0, 0};
  util::size max_size = {0, 0};
  struct {
    bool left = false, right = false, top = false, bottom = false;
    wint_t attribute = 0;
  } border;
  enum { VISIBLE, SCROLL, FIXED } overflow = VISIBLE;
  bool selectable = false;
};
} // namespace duskland::tui