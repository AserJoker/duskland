#pragma once
#include "system_entity.hpp"
#include "system_hud.hpp"
#include "system_map.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class canvas_main : public tui::widget {
private:
  core::auto_release<game::system_hud> _system_hud;
  core::auto_release<game::system_map> _system_map;
  core::auto_release<game::system_entity> _system_entity;

  core::auto_release<entity_base> _player;
  core::auto_release<entity_base> _face;

  core::auto_release<tui::widget> _status;

protected:
  bool on_input(const util::key &key) override;
  void on_update() override;

public:
  canvas_main();
};
} // namespace duskland::game