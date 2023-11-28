#pragma once
#include "core/include/object.hpp"
#include "util/include/rect.hpp"
#include "util/include/tile.hpp"
#include <vector>
namespace duskland::game {
class entity_base : public core::object {
protected:
  util::rect _rect;
  std::vector<util::tile> _tiles;
  bool _visible;

public:
  entity_base();
  const util::rect &get_rect();
  void set_rect(const util::rect &rc);
  const util::tile &get_tile(uint32_t x, uint32_t y);
  void set_tile(uint32_t x, uint32_t y, const util::tile &tile);
  void set_visible(bool visible);
  bool is_visible();
};
} // namespace duskland::game