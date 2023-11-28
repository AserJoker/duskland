#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "tui/include/graphic.hpp"
#include "tui/include/widget.hpp"
namespace duskland::game {
class system_map : public core::object {
public:
  struct block {
    util::tile tile;
  };
  struct chunk : public core::object {
    block data[256];
    uint32_t x;
    uint32_t y;
  };
  class renderer : public tui::widget {
  public:
    void on_render(core::auto_release<tui::graphic> &g);
  };

private:
  std::unordered_map<std::string, core::auto_release<chunk>> _chunks;
  core::auto_release<renderer> _renderer;

public:
  void initialize();
  void uninitialize();
  void on_render(core::auto_release<tui::graphic> &g);
  const core::auto_release<renderer> &get_renderer();
  void set_position(int32_t x, int32_t y);
  core::auto_release<chunk> load_chunk(int32_t chunkx, int32_t chunky);
  void unload_chunk(const core::auto_release<chunk> &chunk);
  core::auto_release<chunk> get_chunk(int32_t chunkx, int32_t chunky);
  bool get_block(int32_t x, int32_t y, block **block);
};
} // namespace duskland::game