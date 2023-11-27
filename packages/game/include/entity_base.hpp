#pragma once
#include "core/include/object.hpp"
#include "util/include/rect.hpp"
namespace duskland::game {
class entity_base : public core::object {
private:
  util::rect _rect;

public:
  entity_base();
};
} // namespace duskland::game