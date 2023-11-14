#pragma once
#include <fmt/format.h>
#include <string>
#include <vector>
namespace duskland::util {
struct key {
  wint_t decode;
  bool shift;
  bool ctrl;
  bool alt;
  bool control;
  std::string name;
  std::vector<wint_t> raw;
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