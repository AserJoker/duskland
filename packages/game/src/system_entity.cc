#include "../include/system_entity.hpp"
#include "core/include/singleton.hpp"
using namespace duskland::game;
using namespace duskland;
void system_entity::initialize() {
  _renderer = new renderer();
  _renderer->get_attribute().position = tui::style::ABSOLUTE;
}
void system_entity::uninitialize() { _renderer = nullptr; }

const std::vector<core::auto_release<entity_base>> &
system_entity::get_entities() {
  return _entites;
};

void system_entity::delete_entity(
    const core::auto_release<entity_base> &entity) {
  std::erase(_entites, entity);
}
const core::auto_release<system_entity::renderer> &
system_entity::get_renderer() {
  return _renderer;
}
void system_entity::renderer::on_render(core::auto_release<tui::graphic> &g) {
  auto system = core::singleton<system_entity>::get();
  for (auto &e : system->_entites) {
    if (e->is_visible()) {
      auto &rc = e->get_rect();
      for (auto y = 0; y < rc.height; y++) {
        for (auto x = 0; x < rc.width; x++) {
          auto &tile = e->get_tile(x, y);
          g->draw(x + rc.x, y + rc.y, tile);
        }
      }
    }
  }
}