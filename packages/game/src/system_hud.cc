#include "../include/system_hud.hpp"
using namespace duskland::game;
using namespace duskland;
void system_hud::initialize() { _renderer = new renderer(); }
void system_hud::uninitialize() { _renderer = nullptr; }

const core::auto_release<system_hud::renderer> &system_hud::get_renderer() {
  return _renderer;
}