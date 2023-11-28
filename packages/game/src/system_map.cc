#include "../include/system_map.hpp"
#include "core/include/singleton.hpp"
#include <fmt/format-inl.h>
using namespace duskland::game;
using namespace duskland;
void system_map::initialize() { _renderer = new renderer(); }
void system_map::uninitialize() { _renderer = nullptr; }
const core::auto_release<system_map::renderer> &system_map::get_renderer() {
  return _renderer;
}
void system_map::renderer::on_render(core::auto_release<tui::graphic> &g) {
  auto system = core::singleton<system_map>::get();
  for (auto &[_, chunk] : system->_chunks) {
    for (auto y = 0; y < 16; y++) {
      for (auto x = 0; x < 16; x++) {
        g->draw(chunk->x * 16 + x, chunk->y * 16 + y,
                chunk->data[y * 16 + x].tile);
      }
    }
  }
}
void system_map::set_position(int32_t x, int32_t y) {
  int32_t chunkx = x / 16;
  int32_t chunky = y / 16;
  std::unordered_map<std::string, core::auto_release<chunk>> chunks;
  for (auto &[_, chunk] : _chunks) {
    if (chunk->x < chunkx - 6 || chunk->y < chunky - 3 ||
        chunk->x > chunkx + 6 || chunk->y > chunky + 3) {
      unload_chunk(chunk);
    } else {
      chunks[fmt::format("{}x{}", chunk->x, chunk->y)] = chunk;
    }
  }
  for (auto yy = -3; yy < 3; yy++) {
    for (auto xx = -6; xx < 6; xx++) {
      auto name = fmt::format("{}x{}", xx + chunkx, yy + chunky);
      if (!chunks.contains(name)) {
        chunks[name] = load_chunk(chunkx + xx, chunky + yy);
      }
    }
  }
  _chunks = chunks;
}
core::auto_release<system_map::chunk> system_map::load_chunk(int32_t chunkx,
                                                             int32_t chunky) {
  auto res = new chunk;
  res->x = chunkx;
  res->y = chunky;
  for (auto y = 0; y < 16; y++) {
    for (auto x = 0; x < 16; x++) {
      res->data[y * 16 + x] = {L'.', "grass"};
    }
  }
  return res;
}
void system_map::unload_chunk(
    const core::auto_release<system_map::chunk> &chunk) {}
core::auto_release<system_map::chunk> system_map::get_chunk(int32_t chunkx,
                                                            int32_t chunky) {
  auto name = fmt::format("{}x{}", chunkx, chunky);
  if (_chunks.contains(name)) {
    return _chunks.at(name);
  }
  return nullptr;
}

bool system_map::get_block(int32_t x, int32_t y, system_map::block **block) {
  auto chunk = get_chunk(x / 16, y / 16);
  if (chunk != nullptr) {
    *block = &(chunk->data[y % 16 * 16 + x % 16]);
    return true;
  }
  return false;
}