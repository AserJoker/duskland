#include "../include/entity_base.hpp"
using namespace duskland::game;
using namespace duskland;
entity_base::entity_base() : _visible(true) {
  _rect = {0, 0, 1, 1};
  _tiles.push_back(util::tile{L'.', ""});
}
const util::rect &entity_base::get_rect() { return _rect; };
void entity_base::set_rect(const util::rect &rc) {
  std::vector<util::tile> tiles;
  for (auto y = 0; y < rc.height; y++) {
    for (auto x = 0; x < rc.width; x++) {
      if (x < _rect.width && y < _rect.height) {
        tiles.push_back(get_tile(x, y));
      } else {
        _tiles.push_back(util::tile{L' ', ""});
      }
    }
  }
  _tiles = tiles;
  _rect = rc;
};
const util::tile &entity_base::get_tile(uint32_t x, uint32_t y) {
  return _tiles[y * _rect.width + x];
}
void entity_base::set_tile(uint32_t x, uint32_t y, const util::tile &tile) {
  _tiles[y * _rect.width + x] = tile;
};
void entity_base::set_visible(bool visible) { _visible = visible; }
bool entity_base::is_visible() { return _visible; }