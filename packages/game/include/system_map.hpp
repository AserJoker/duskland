#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "tui/include/graphic.hpp"
namespace duskland::game {
class system_map : public core::object {
public:
  void initialize();
  void uninitialize();
  void on_render(core::auto_release<tui::graphic> &g);
};
} // namespace duskland::game