#include "system/input.hpp"
#include <cjson/cJSON.h>
#include <curses.h>
#include <fstream>
#include <iostream>
using namespace duskland::system;
input::input() {}
void input::load(const std::string &content) {
  auto root = cJSON_Parse(content.c_str());
  auto child = root->child;
  while (child) {
    util::key k;
    k.decode = 0;
    k.name = child->string;
    k.alt = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "alt"));
    k.shift = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "shift"));
    k.ctrl = cJSON_GetNumberValue(cJSON_GetObjectItem(child, "ctrl"));
    auto data = cJSON_GetObjectItem(child, "data");
    auto len = cJSON_GetArraySize(data);
    for (auto i = 0; i < len; i++) {
      k.raw.push_back(cJSON_GetNumberValue(cJSON_GetArrayItem(data, i)));
    }
    _keylist.push_back(k);
    child = child->next;
  }
  cJSON_free(root);
}
bool input::read(std::vector<util::key> &output) {
  std::vector<wint_t> codes;
  for (;;) {
    wint_t c;
    if (get_wch(&c) == ERR) {
      c = ERR;
    }
    if (c == ERR) {
      break;
    }
    codes.push_back(c);
  }
  if (codes.empty()) {
    return false;
  }
  for (auto &k : _keylist) {
    if (std::equal(k.raw.begin(), k.raw.end(), codes.begin(), codes.end())) {
      output.push_back(k);
      return true;
    }
  }
  for (auto &code : codes) {
    output.push_back({.decode = code,
                      .shift = false,
                      .ctrl = false,
                      .alt = false,
                      .name = fmt::format("<{}>", (char)code),
                      .raw = {code}});
    return true;
  }
  return false;
}
void input::initialize() {
  nodelay(stdscr, TRUE);
  set_escdelay(1);
}
void input::uninitialize() {
  clrtoeol();
  refresh();
}