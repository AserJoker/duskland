#include "../include/brush.hpp"
#include <cjson/cJSON.h>
#include <codecvt>
#include <curses.h>
#include <locale>
#include <queue>
#include <sstream>
using namespace duskland::tui;
void resolve_nodes(std::unordered_map<std::string, wchar_t> &store, cJSON *obj,
                   const std::string &name) {
  if (cJSON_IsString(obj)) {
    std::wstring wvalue =
        std::wstring_convert<std::codecvt_utf8<wchar_t>>{}.from_bytes(
            obj->valuestring);
    store[name] = wvalue.at(0);
  } else {
    auto child = obj->child;
    while (child) {
      auto child_name = name;
      if (child_name.empty()) {
        child_name = child->string;
      } else {
        child_name += ".";
        child_name += child->string;
      }
      resolve_nodes(store, child, child_name);
      child = child->next;
    }
  }
}
void brush::load_attribute(const std::string &source_json) {
  auto root = cJSON_Parse(source_json.c_str());
  auto palette = cJSON_GetObjectItem(root, "palette");
  auto pairs = cJSON_GetObjectItem(root, "attributes");
  std::unordered_map<std::string, short> colors;
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
    _attributes.insert({name, attr});
    pair_index++;
    child = child->next;
  }
  cJSON_free(root);
}
wint_t brush::query_attribute(const std::string &name) {
  return _attributes.at(name);
}
void brush::load_symbol(const std::string &source_json) {
  auto root = cJSON_Parse(source_json.c_str());
  resolve_nodes(_symbols, root, "");
  cJSON_free(root);
}
wchar_t brush::query_symbol(const std::string &name) {
  try {
    return _symbols.at(name);
  } catch (std::exception &e) {
    throw e;
  }
}