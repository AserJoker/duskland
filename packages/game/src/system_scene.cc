#include "../include/system_scene.hpp"
#include "game/include/scene_main.hpp"
using namespace duskland::game;
void system_scene::initialize(core::auto_release<tui::widget> &root) {
  _layout = new tui::windows();
  root->add_child(_layout.get());
  set_scene(new scene_main());
}
void system_scene::set_scene(const core::auto_release<scene_base> &scene) {
  if (_scene != nullptr) {
    _scene->uninitialize();
  }
  _scene = scene;
  _scene->initialize(_layout);
}