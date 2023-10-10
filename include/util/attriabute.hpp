#pragma once
#include "core/object.hpp"
#include <map>
#include <ncurses.h>
#include <string>
#define COLOR_PAIR_INDEX(fg, bg)                                               \
  ((((uint16_t)fg) << 4) | (((uint16_t)bg) & 0xf))

namespace duskland::util {
class attribute : public core::object {
private:
  std::map<std::string, uint32_t> _attributes;

public:
  void initialize();
  uint32_t attr(const std::string &name);
  void attr(const std::string &name, const uint16_t &fg, const uint16_t &bg,
            const uint32_t &style = 0);
};
} // namespace duskland::util