#pragma once
#include <ncurses.h>
namespace duskland::util {
struct border {
  bool left;
  bool right;
  bool top;
  bool bottom;
};
}; // namespace duskland::util