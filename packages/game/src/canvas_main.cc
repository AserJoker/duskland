#include "../include/canvas_main.hpp"
#include "core/include/singleton.hpp"
using namespace duskland::game;
canvas_main::canvas_main() {
  _system_entity = core::singleton<system_entity>::get();
  _system_hud = core::singleton<system_hud>::get();
  _system_map = core::singleton<system_map>::get();
  _system_map->set_position(0, 0);

  _player = _system_entity->create_entity<entity_base>();
  _face = _system_entity->create_entity<entity_base>();
  _player->set_tile(0, 0, {L'@', "player"});
  _face->set_tile(0, 0, {0, "face"});
  _face->set_rect({1, 0, 1, 1});
  get_attribute().size.width = -1;
  get_attribute().size.height = -1;
  add_child(_system_map->get_renderer().get());
  add_child(_system_entity->get_renderer().get());
  add_child(_system_hud->get_renderer().get());
  request_update();
}
bool canvas_main::on_input(const util::key &key) {
  request_update();
  if (key.name == "<a>") {
    auto &frc = _face->get_rect();
    auto &rc = _player->get_rect();
    if (frc.x == rc.x - 1 && frc.y == rc.y) {
      _player->set_rect({rc.x - 1, rc.y, rc.width, rc.height});
      _face->set_rect({frc.x - 1, frc.y, rc.width, rc.height});
    } else {
      _face->set_rect({rc.x - 1, rc.y, 1, 1});
    }
    return true;
  }
  if (key.name == "<d>") {
    auto &frc = _face->get_rect();
    auto &rc = _player->get_rect();
    if (frc.x == rc.x + 1 && frc.y == rc.y) {
      _player->set_rect({rc.x + 1, rc.y, rc.width, rc.height});
      _face->set_rect({frc.x + 1, frc.y, rc.width, rc.height});
    } else {
      _face->set_rect({rc.x + 1, rc.y, 1, 1});
    }
    return true;
  }
  if (key.name == "<w>") {
    auto &frc = _face->get_rect();
    auto &rc = _player->get_rect();
    if (frc.x == rc.x && frc.y == rc.y - 1) {
      _player->set_rect({rc.x, rc.y - 1, rc.width, rc.height});
      _face->set_rect({frc.x, frc.y - 1, rc.width, rc.height});
    } else {
      _face->set_rect({rc.x, rc.y - 1, 1, 1});
    }
    return true;
  }
  if (key.name == "<s>") {
    auto &frc = _face->get_rect();
    auto &rc = _player->get_rect();
    if (frc.x == rc.x && frc.y == rc.y + 1) {
      _player->set_rect({rc.x, rc.y + 1, rc.width, rc.height});
      _face->set_rect({frc.x, frc.y + 1, rc.width, rc.height});
    } else {
      _face->set_rect({rc.x, rc.y + 1, 1, 1});
    }
    return true;
  }
  if (key.name == "<e>") {
    auto &rc = _face->get_rect();
    system_map::block *block = nullptr;
    _system_map->get_block(rc.x, rc.y, &block);
    block->tile.attr = "dirt";
    return true;
  }
  return tui::widget::on_input(key);
}