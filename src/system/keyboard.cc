﻿#include "system/keyboard.hpp"
#include <cjson/cJSON.h>
#include <curses.h>
#include <fstream>
#include <iostream>
using namespace duskland::system;
keyboard::keyboard() {}
void keyboard::load(const std::string &content) {
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
bool keyboard::read(std::vector<util::key> &output) {
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
      k.control = true;
      output.push_back(k);
      return true;
    }
  }
  for (auto &code : codes) {
    output.push_back({.decode = code,
                      .shift = false,
                      .ctrl = false,
                      .alt = false,
                      .control = false,
                      .name = fmt::format("<{}>", (char)code),
                      .raw = {code}});
  }
  if(!output.empty()){
    return true;
  }
  return false;
}
void keyboard::initialize() {
  nodelay(stdscr, TRUE);
  set_escdelay(1);
}
void keyboard::uninitialize() {
  clrtoeol();
  refresh();
}