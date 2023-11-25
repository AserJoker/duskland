#include "../include/canvas_main.hpp"
#include "core/include/singleton.hpp"
using namespace duskland::game;
void canvas_main::on_render(core::auto_release<tui::graphic> &g) {
  _system_map->on_render(g);
  _system_entity->on_render(g);
  _system_hud->on_render(g);
  set_timer(L"render", 250);
}
canvas_main::canvas_main() {
  _system_entity = core::singleton<system_entity>::get();
  _system_map = core::singleton<system_map>::get();
  _system_hud = core::singleton<system_hud>::get();
}
void canvas_main::on_timer(const std::wstring &name) {
  if (name == L"render") {
    request_update();
  }
}