#include "script/runtime.hpp"
#include <stdexcept>
using namespace duskland::script;
runtime::runtime() {
  _state = lua_open();
  luaopen_base(_state);
  luaopen_table(_state);
  luaopen_math(_state);
  luaopen_string(_state);
}
runtime::~runtime() { lua_close(_state); }
void runtime::eval(const std::string &source) {
  auto top = lua_gettop(_state);
  auto err = luaL_loadstring(_state, source.c_str());
  if (err != 0) {
    auto message = lua_tostring(_state, -1);
    throw std::runtime_error(message);
  }
  err = lua_pcall(_state, 0, 0, 0);
  if (err != 0) {
    auto message = lua_tostring(_state, -1);
    throw std::runtime_error(message);
  }
  lua_settop(_state, top);
}