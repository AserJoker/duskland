#pragma once
#include "core/object.hpp"
#include <map>
#include <string>
namespace duskland::util {
class color : public core::object {
private:
  std::map<std::string, wint_t> _colors;

public:
  void load(const std::string &source_json);
  wint_t query(const std::string &name);
};
} // namespace duskland::util