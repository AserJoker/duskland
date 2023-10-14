#pragma once
#include "core/object.hpp"
#include "util/key.hpp"
#include <map>
#include <ncurses.h>
#include <vector>
namespace duskland::system {
class system_input : public core::object {
private:
  wint_t decode(const std::vector<int> &codes);
  std::map<wint_t, std::vector<int>> _key_binding;

public:
  void initialize();
  void uninitialize();
  wint_t read();
};
}; // namespace duskland::system