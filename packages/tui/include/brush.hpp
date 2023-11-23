#pragma once
#include "core/include/object.hpp"
#include <string>
#include <unordered_map>
namespace duskland::tui {
class brush : public core::object {
private:
  std::unordered_map<std::string, wint_t> _attributes;
  std::unordered_map<std::string, wchar_t> _symbols;

public:
  void load_attribute(const std::string &source_json);
  void load_symbol(const std::string &source_json);
  wint_t query_attribute(const std::string &name);
  wchar_t query_symbol(const std::string &name);
};
} // namespace duskland::tui