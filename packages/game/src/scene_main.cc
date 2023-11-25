#include "../include/scene_main.hpp"
#include "game/include/canvas_main.hpp"
using namespace duskland::game;
using namespace duskland;
void scene_main::initialize(core::auto_release<tui::windows> &layout) {
  auto node = new tui::windows::node();
  node->identity = "canvas-main";
  layout->set_root(node);
  auto win = layout->get_window("canvas-main");
  win->add_child(new canvas_main());
};
void scene_main::uninitialize(){};