#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "scene_base.hpp"
#include "tui/include/widget.hpp"
#include "tui/include/windows.hpp"
namespace duskland::game {
class system_scene : public core::object {
private:
  core::auto_release<tui::windows> _layout;
  core::auto_release<scene_base> _scene;

public:
  void initialize(core::auto_release<tui::widget> &root);
  void set_scene(const core::auto_release<scene_base> &scene);
};
} // namespace duskland::game