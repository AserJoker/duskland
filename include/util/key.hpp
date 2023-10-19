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