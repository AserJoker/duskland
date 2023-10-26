#include "util/injector.hpp"
using namespace duskland::util;
void injector::initialize() {
  for (int16_t fg = 0; fg < 8; fg++) {
    for (int16_t bg = 0; bg < 8; bg++) {
      init_pair(COLOR_PAIR_INDEX(fg, bg), fg, bg);
    }
  }
  attr("injector.default", COLOR_WHITE, COLOR_BLACK);
  keymap("injector.default", "");
  style("injector.default", L' ');
  feature("injector.default", false);
}
const uint32_t &injector::attr(const std::string &name) const {
  if (_attributes.contains(name)) {
    return _attributes.at(name);
  }
  return _attributes.at("injector.default");
}
void injector::attr(const std::string &name, const uint16_t &fg,
                    const uint16_t &bg, const uint32_t &style) {
  _attributes[name] = COLOR_PAIR(COLOR_PAIR_INDEX(fg, bg)) | style;
}
void injector::keymap(const std::string &name, const std::string &key) {
  _keymaps[name] = key;
}
const std::string &injector::keymap(const std::string &name) const {
  if (_keymaps.contains(name)) {
    return _keymaps.at(name);
  }
  return _keymaps.at("injector.default");
}

void injector::style(const std::string &name, const wchar_t &style) {
  _styles[name] = style;
}
const wchar_t &injector::style(const std::string &name) {
  if (_styles.contains(name)) {
    return _styles.at(name);
  }
  return _styles.at("injector.default");
}
bool injector::attr_exist(const std::string &name) const {
  return _attributes.contains(name);
}
bool injector::keymap_exist(const std::string &name) const {
  return _keymaps.contains(name);
}
bool injector::style_exist(const std::string &name) const {
  return _styles.contains(name);
}

const std::vector<std::string> &injector::args() const { return _args; };
std::vector<std::string> &injector::args() { return _args; }
const bool &injector::feature(const std::string &name) const {
  if (_features.contains(name)) {
    return _features.at(name);
  }
  return _features.at("injector.default");
}
void injector::feature(const std::string &name, const bool &val) {
  _features[name] = val;
}