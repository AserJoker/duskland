#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "tui/include/windows.hpp"
namespace duskland::game {
class scene_base : public core::object {
public:
  virtual void initialize(core::auto_release<tui::windows> &layout) = 0;
  virtual void uninitialize() = 0;
};
} // namespace duskland::game