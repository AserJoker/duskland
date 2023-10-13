#pragma once
#include "core/object.hpp"
#include <map>
#include <ncurses.h>
#include <string>
#define COLOR_PAIR_INDEX(fg, bg) (int16_t)(fg * 8 + bg)

namespace duskland::util {
class config : public core::object {
private:
  std::map<std::string, uint32_t> _attributes;
  std::map<std::string, chtype> _keymaps;
  std::map<std::string, wchar_t> _styles;

public:
  void initialize();
  const uint32_t &attr(const std::string &name) const;
  void attr(const std::string &name, const uint16_t &fg, const uint16_t &bg,
            const uint32_t &style = 0);
  void keymap(const std::string &name, const chtype &key);
  const chtype &keymap(const std::string &name) const;

  void style(const std::string &name, const wchar_t &style);
  const wchar_t &style(const std::string &name);
};
} // namespace duskland::util