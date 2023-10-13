#pragma once
#include "core/object.hpp"
#include <ncurses.h>
namespace duskland::system {
class system_input : public core::object {
private:
  wint_t decode(const wint_t& ch);
public:
  void initialize();
  void uninitialize();
  wint_t read();
};
}; // namespace duskland::system