#pragma once
#include "core/include/object.hpp"
#include "util/include/key.hpp"
namespace duskland::runtime {
class keyboard : public core::object {
private:
  std::vector<util::key> _keylist;

public:
  void set_keymap(const util::key &key);
  bool read(std::vector<util::key> &output);
  void load(const std::string &keymap_json);
  void initialize();
  void uninitialize();
  keyboard();
};
} // namespace duskland::runtime