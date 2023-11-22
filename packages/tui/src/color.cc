#include "../include/attribute.hpp"
#include <cjson/cJSON.h>
#include <curses.h>
#include <sstream>
using namespace duskland::tui;
void attribute::load(const std::string &source_json) {
  auto root = cJSON_Parse(source_json.c_str());
  auto palette = cJSON_GetObjectItem(root, "palette");
  auto pairs = cJSON_GetObjectItem(root, "pairs");
  std::map<std::string, short> colors;
  auto child = palette->child;
  while (child) {
    auto name = child->string;
    auto s_val = child->valuestring;
    std::stringstream ss(s_val);
    uint32_t color;
    ss >> std::hex >> color;
    auto index = colors.size();
    auto r = (color >> 16) & 0xff;
    auto g = (color >> 8) & 0xff;
    auto b = color & 0xff;
    r = r * 1.0f / 0xff * 1000;
    g = g * 1.0f / 0xff * 1000;
    b = b * 1.0f / 0xff * 1000;
    init_color(index, r, g, b);
    colors.insert({name, index});
    child = child->next;
  }
  child = pairs->child;
  short pair_index = 0;
  while (child) {
    auto name = child->string;
    wint_t attr = 0;
    auto s_fg = cJSON_GetObjectItem(child, "fg")->valuestring;
    auto s_bg = cJSON_GetObjectItem(child, "bg")->valuestring;
    auto fg = colors.at(s_fg);
    auto bg = colors.at(s_bg);
    init_pair(pair_index, fg, bg);
    attr = COLOR_PAIR(pair_index);
    if (cJSON_IsTrue(cJSON_GetObjectItem(child, "dim"))) {
      attr |= WA_DIM;
    }
    if (cJSON_IsTrue(cJSON_GetObjectItem(child, "standout"))) {
      attr |= WA_STANDOUT;
    }
    if (cJSON_IsTrue(cJSON_GetObjectItem(child, "underline"))) {
      attr |= WA_UNDERLINE;
    }
    if (cJSON_IsTrue(cJSON_GetObjectItem(child, "bold"))) {
      attr |= WA_BOLD;
    }
    _colors.insert({name, attr});
    pair_index++;
    child = child->next;
  }
  cJSON_free(root);
}
wint_t attribute::query(const std::string &name) {
  return _colors.at(name); }