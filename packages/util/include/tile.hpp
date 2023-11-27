#pragma once
#include <cstdint>
#include <string>
namespace duskland::util {
struct tile {
  wchar_t ch;
  std::string attr;
};
} // namespace duskland::util