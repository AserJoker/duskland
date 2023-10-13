#include "util/config.hpp"
using namespace duskland::util;
void config::initialize() {
  for (int16_t fg = 0; fg < 8; fg++) {
    for (int16_t bg = 0; bg < 8; bg++) {
      init_pair(COLOR_PAIR_INDEX(fg, bg), fg, bg);
    }
  }
  attr("config.default", COLOR_WHITE, COLOR_BLACK);
  keymap("config.default", 0);
  style("config.default", L' ');
}
const uint32_t &config::attr(const std::string &name) const {
  if (_attributes.contains(name)) {
    return _attributes.at(name);
  }
  return _attributes.at("config.default");
}
void config::attr(const std::string &name, const uint16_t &fg,
                  const uint16_t &bg, const uint32_t &style) {
  _attributes[name] = COLOR_PAIR(COLOR_PAIR_INDEX(fg, bg)) | style;
}
void config::keymap(const std::string &name, const chtype &key) {
  _keymaps[name] = key;
}
const chtype &config::keymap(const std::string &name) const {
  if (_keymaps.contains(name)) {
    return _keymaps.at(name);
  }
  return _keymaps.at("config.default");
}

void config::style(const std::string &name, const wchar_t &style) {
  _styles[name] = style;
}
const wchar_t &config::style(const std::string &name) {
  if (_styles.contains(name)) {
    return _styles.at(name);
  }
  return _styles.at("config.default");
}