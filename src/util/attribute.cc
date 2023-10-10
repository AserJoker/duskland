#include "util/attriabute.hpp"
using namespace duskland::util;
void attribute::initialize() {
  for (int16_t fg = 0; fg < 8; fg++) {
    for (int16_t bg = 0; bg < 8; bg++) {
      init_pair(COLOR_PAIR_INDEX(fg, bg), fg, bg);
    }
  }
  init_pair(0xff, COLOR_RED, COLOR_BLACK);
  attr("attribute.default", COLOR_WHITE, COLOR_BLACK);
}
uint32_t attribute::attr(const std::string &name) {
  if (_attributes.contains(name)) {
    return _attributes.at(name);
  }
  return _attributes.at("attribute.default");
}
void attribute::attr(const std::string &name, const uint16_t &fg,
                     const uint16_t &bg, const uint32_t &style) {
  _attributes[name] = COLOR_PAIR(COLOR_PAIR_INDEX(fg, bg)) | style;
}