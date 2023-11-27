#include "../include/canvas_main.hpp"
#include "core/include/singleton.hpp"
#include "game/include/renderer_entity.hpp"
#include "game/include/renderer_hud.hpp"
#include "game/include/renderer_map.hpp"
using namespace duskland::game;
canvas_main::canvas_main() {
  add_child(new renderer_map());
  add_child(new renderer_entity());
  add_child(new renderer_hud());
}