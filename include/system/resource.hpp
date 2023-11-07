#pragma once
#include "core/object.hpp"
#include <map>
#include <string>
#include <vector>
namespace duskland::system {
class resource : public core::object {
private:
  std::map<std::string, std::string> _resources;
  void load_resource(const std::string &path,
                     const std::vector<std::string> &p);

public:
  void load(const std::string &root);
  std::vector<char> query(const std::string &name);
};
} // namespace duskland::system