#pragma once
#include <ncurses.h>
namespace duskland::util {
struct border_style {
  chtype ls;
  chtype rs;
  chtype ts;
  chtype bs;
  chtype tl;
  chtype tr;
  chtype bl;
  chtype br;
};
struct border {
  bool left;
  bool right;
  bool top;
  bool bottom;
};
}; // namespace duskland::util