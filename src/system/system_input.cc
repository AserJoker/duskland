#include "system/system_input.hpp"
#include <ncurses.h>
using namespace duskland::system;
using namespace duskland;
void system_input::initialize() { nodelay(stdscr, TRUE); }
void system_input::uninitialize() { nodelay(stdscr, FALSE); }
wint_t system_input::read() {
  wint_t ch;
  if (wget_wch(stdscr, &ch) != ERR) {
    return decode(ch);
  }
  return -1;
}
wint_t system_input::decode(const wint_t &ch) {
  wint_t controls[20] = {0};
  wint_t *selector = &controls[0];
  if (ch == 0x1b) {
    wint_t control;
    while (wget_wch(stdscr, &control) != ERR) {
      *selector++ = control;
    }
    *selector = 0;
    if (controls[0] == 0x4f) {
      switch (controls[1]) {
      case 0x41:
        return KEY_UP;
      case 0x42:
        return KEY_DOWN;
      case 0x43:
        return KEY_RIGHT;
      case 0x44:
        return KEY_LEFT;
      case 0x48:
        return KEY_HOME;
      case 0x46:
        return KEY_END;
      }
    } else if (controls[0] == 0x5b) {
      if (controls[1] == 0x32) {
        if (controls[3] == 0x7e) {
          return KEY_F0 + controls[2] - 0x28;
        }
      }
      if (controls[1] == 0x32 && controls[2] == 0x7e) {
        return KEY_IL;
      }
      if (controls[1] == 0x33 && controls[2] == 0x7e) {
        return KEY_DL;
      }
    }
  }
  return ch;
}