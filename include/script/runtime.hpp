#pragma once
#include "core/object.hpp"
#include <functional>
#include <lua5.1/lua.hpp>
#include <string>
namespace duskland::script {
class runtime : public core::object {
private:
  lua_State *_state;

public:
  runtime();
  ~runtime();
  void eval(const std::string &source);
};
}; // namespace duskland::script