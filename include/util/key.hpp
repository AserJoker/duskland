#pragma once
#include <map>
#include <ncurses.h>
#include <vector>
namespace duskland::util {
struct command {
  std::vector<wint_t> raw;
  wint_t decode;
};
extern const std::map<wint_t, std::vector<wint_t>> key_code_mapping;
} // namespace duskland::util

#define VKEY_ESC 0x0
#define VKEY_BACKSPACE 0x1
#define VKEY_TAB 0x2
#define VKEY_ENTER 0x3
#define VKEY_LEFT 0x4
#define VKEY_RIGHT 0x5
#define VKEY_UP 0x6
#define VKEY_DOWN 0x7
#define VKEY_HOME 0x8
#define VKEY_END 0x9
#define VKEY_INSERT 0xa
#define VKEY_DELETE 0xb
#define VKEY_PAGE_UP 0xc
#define VKEY_PAGE_DOWN 0xd
#define VKEY_F0 0xe
#define VKEY_F(n) (VKEY_F0 + n)

#define VKEY_SHIFT_TAB 0x1b
#define VKEY_SHIFT_LEFT 0x1c
#define VKEY_SHIFT_RIGHT 0x1d
#define VKEY_SHIFT_UP 0x1e
#define VKEY_SHIFT_DOWN 0x1f
#define VKEY_SHIFT_HOME 0x20
#define VKEY_SHIFT_END 0x21
#define VKEY_SHIFT_DELETE 0x22
#define VKEY_SHIFT_INSERT 0x23
#define VKEY_SHIFT_PAGE_UP 0x24
#define VKEY_SHIFT_PAGE_DOWN 0x25
#define VKEY_SHIFT_F0 0x26
#define VKEY_SHIFT_F(n) (VKEY_SHIFT_F0 + n)

#define VKEY_CTRL_TAB 0x33
#define VKEY_CTRL_LEFT 0x34
#define VKEY_CTRL_RIGHT 0x35
#define VKEY_CTRL_UP 0x36
#define VKEY_CTRL_DOWN 0x37
#define VKEY_CTRL_HOME 0x38
#define VKEY_CTRL_END 0x39
#define VKEY_CTRL_DELETE 0x3a
#define VKEY_CTRL_INSERT 0x3b
#define VKEY_CTRL_PAGE_UP 0x3c
#define VKEY_CTRL_PAGE_DOWN 0x3d
#define VKEY_CTRL_F0 0x3e
#define VKEY_CTRL_F(n) (VKEY_CTRL_F0 + n)