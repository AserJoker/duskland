#pragma once
#include "core/object.hpp"
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>

namespace duskland::util {
class injector : public core::object {
private:
  std::map<std::string, uint32_t> _attributes;
  std::map<std::string, std::string> _keymaps;
  std::map<std::string, wchar_t> _styles;
  std::map<std::string, bool> _features;
  std::vector<std::string> _args;

public:
  void initialize();
  bool attr_exist(const std::string &name) const;
  const uint32_t &attr(const std::string &name) const;
  void attr(const std::string &name, const uint16_t &fg, const uint16_t &bg,
            const uint32_t &style = 0);
  bool keymap_exist(const std::string &name) const;
  void keymap(const std::string &name, const std::string &key);
  const std::string &keymap(const std::string &name) const;

  bool style_exist(const std::string &name) const;
  void style(const std::string &name, const wchar_t &style);
  const wchar_t &style(const std::string &name);

  const bool &feature(const std::string &name) const;
  void feature(const std::string &name, const bool &value);

  const std::vector<std::string> &args() const;
  std::vector<std::string> &args();
};
} // namespace duskland::util