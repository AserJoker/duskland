#pragma once
#include <ncurses.h>
namespace duskland::util {
struct border_style {
  wchar_t ls;
  wchar_t rs;
  wchar_t ts;
  wchar_t bs;
  wchar_t tl;
  wchar_t tr;
  wchar_t bl;
  wchar_t br;
};
struct border {
  bool left;
  bool right;
  bool top;
  bool bottom;
};
#define BORDER_DEFAULT                                                         \
  { L'│', L'│', L'─', L'─', L'┌', L'┐', L'└', L'┘' }
#define BORDER_BOLD                                                            \
  { L'┃', L'┃', L'━', L'━', L'┏', L'┓', L'┗', L'┛' }
}; // namespace duskland::util