#pragma once
#include <cstdint>
namespace duskland::util {
struct border_info {
  bool left;
  bool right;
  bool top;
  bool bottom;
};
struct layout_info {
  uint32_t left;
  uint32_t right;
  uint32_t top;
  uint32_t bottom;
  int32_t x;
  int32_t y;
};
}; // namespace duskland::util