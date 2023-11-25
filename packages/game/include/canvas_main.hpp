#pragma once
#include "system_entity.hpp"
#include "system_hud.hpp"
#include "system_map.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class canvas_main : public tui::widget {
private:
  core::auto_release<system_hud> _system_hud;
  core::auto_release<system_map> _system_map;
  core::auto_release<system_entity> _system_entity;

protected:
  void on_render(core::auto_release<tui::graphic> &g) override;
  void on_timer(const std::wstring &name) override;

public:
  canvas_main();
};
} // namespace duskland::game