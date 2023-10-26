#pragma once
#include <fmt/format.h>
#include <ncurses.h>
#include <string>
#include <vector>
namespace duskland::util {
struct key {
  wint_t decode;
  bool shift;
  bool ctrl;
  bool alt;
  std::vector<wint_t> raw;
  key() : decode(0) {}
  key(const wint_t &decode, const bool &shift = false, const bool &ctrl = false,
      const bool &alt = false, const std::vector<wint_t> &raw = {})
      : decode(decode), shift(shift), ctrl(ctrl), alt(alt), raw(raw) {}
  std::string name() const {
    std::string basename;

    if (decode == '\t') {
      basename = "tab";
    } else if (decode == '\n') {
      basename = "enter";
    } else if (decode == KEY_LEFT) {
      basename = "left";
    } else if (decode == KEY_RIGHT) {
      basename = "right";
    } else if (decode == KEY_UP) {
      basename = "up";
    } else if (decode == KEY_DOWN) {
      basename = "down";
    } else if (decode == KEY_IC) {
      basename = "insert";
    } else if (decode == KEY_DC) {
      basename = "delete";
    } else if (decode == KEY_HOME) {
      basename = "home";
    } else if (decode == KEY_END) {
      basename = "end";
    } else if (decode == KEY_NPAGE) {
      basename = "pgdn";
    } else if (decode == KEY_PPAGE) {
      basename = "pgup";
    } else if (decode >= KEY_F(1) && decode <= KEY_F(12)) {
      basename = fmt::format("f{}", decode - KEY_F(0));
    } else if (decode < 128) {
      basename += (char)decode;
    }
    if (shift) {
      basename = fmt::format("s-{}", basename);
    }
    if (alt) {
      basename = fmt::format("m-{}", basename);
    }
    if (ctrl) {
      basename = fmt::format("c-{}", basename);
    }
    return fmt::format("<{}>", basename);
  }
};
struct key_encoding {
  enum { data, set } type;
  key key_code;
  std::vector<std::pair<wint_t, key_encoding>> key_set;
  key_encoding() : type(set), key_set({}) {}
  key_encoding(const key &val) : type(data), key_code(val) {}
  key_encoding(const std::vector<std::pair<wint_t, key_encoding>> &key_set)
      : type(set), key_set(key_set) {}
  key_encoding(const key_encoding &another) {
    if (another.type == data) {
      type = data;
      key_code = another.key_code;
    } else {
      type = set;
      key_set = another.key_set;
    }
  }
  key_encoding operator[](const uint32_t &id) const {
    if (type == set) {
      for (auto &[k, v] : key_set) {
        if (k == id) {
          return v;
        }
      }
    }
    return {};
  }
  key code() const {
    if (type == data) {
      return key_code;
    } else {
      return {0, false, false, false};
    }
  }
};
extern const util::key_encoding keymap;
} // namespace duskland::util