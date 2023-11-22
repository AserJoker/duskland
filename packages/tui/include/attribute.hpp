#pragma once
#include "core/include/object.hpp"
#include <map>
#include <string>
namespace duskland::tui {
class attribute : public core::object {
private:
  std::map<std::string, wint_t> _colors;

public:
  void load(const std::string &source_json);
  wint_t query(const std::string &name);
};
} // namespace duskland::util