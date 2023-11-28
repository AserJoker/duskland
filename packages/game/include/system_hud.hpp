#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "tui/include/graphic.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class system_hud : public core::object {
public:
  class renderer : public tui::widget {};

private:
  core::auto_release<renderer> _renderer;

public:
  void initialize();
  void uninitialize();
  const core::auto_release<renderer> &get_renderer();
};
} // namespace duskland::game