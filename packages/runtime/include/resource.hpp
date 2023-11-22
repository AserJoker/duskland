#pragma once
#include "core/include/object.hpp"
#include <map>
#include <string>
#include <vector>
namespace duskland::runtime {
class resource : public core::object {
private:
  std::map<std::string, std::string> _resources;
  void load_resource(const std::string &path,
                     const std::vector<std::string> &p);

public:
  void load(const std::string &root);
  std::vector<char> query(const std::string &name);
};
} // namespace duskland::runtime