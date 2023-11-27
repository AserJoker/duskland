#pragma once
#include "system_entity.hpp"
#include "system_hud.hpp"
#include "system_map.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class canvas_main : public tui::widget {

public:
  canvas_main();
};
} // namespace duskland::game