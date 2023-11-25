#pragma once
#include "core/include/object.hpp"
#include "core/include/singleton.hpp"
#include "game/include/system_entity.hpp"
#include "game/include/system_hud.hpp"
#include "game/include/system_map.hpp"
#include "game/include/system_scene.hpp"
#include "keyboard.hpp"
#include "resource.hpp"
#include "tui/include/graphic.hpp"
#include "tui/include/widget.hpp"
#include "util/include/key.hpp"
#include <string>
#include <vector>
namespace duskland::runtime {
class application : public core::object {

private:
  bool _is_running;
  core::auto_release<tui::graphic> _graphic;
  core::auto_release<keyboard> _keyboard;
  core::auto_release<resource> _resource;
  core::auto_release<tui::brush> _brush;

  core::auto_release<tui::widget> _root;

  core::auto_release<game::system_scene> _system_scene;
  core::auto_release<game::system_hud> _system_hud;
  core::auto_release<game::system_map> _system_map;
  core::auto_release<game::system_entity> _system_entity;

public:
  int run();
  application();
  ~application() override;
  void initialize(int argc, char *argv[]);
  void uninitialize();
  void exit();
  void on_command(const util::key &cmd);
};
} // namespace duskland::runtime