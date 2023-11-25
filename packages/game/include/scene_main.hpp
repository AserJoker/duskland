#pragma once
#include "scene_base.hpp"
namespace duskland::game {
class scene_main : public scene_base {
public:
  virtual void initialize(core::auto_release<tui::windows> &layout);
  virtual void uninitialize();
};
} // namespace duskland::game