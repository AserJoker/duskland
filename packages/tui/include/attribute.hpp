#pragma once
#include "core/include/object.hpp"
#include <unordered_map>
#include <string>
namespace duskland::tui {
class attribute : public core::object {
private:
  std::unordered_map<std::string, wint_t> _attributes;

public:
  void load(const std::string &source_json);
  wint_t query(const std::string &name);
};
} // namespace duskland::tui