#pragma once
#include "core/object.hpp"
#include "util/key.hpp"
namespace duskland::system {
class input : public core::object {
private:
  std::vector<util::key> _keylist;

public:
  void set_keymap(const util::key &key);
  bool read(std::vector<util::key> &output);
  void load(const std::string &keymap);
  void initialize();
  void uninitialize();
  input();
};
} // namespace duskland::system