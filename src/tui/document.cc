#include "tui/document.hpp"
using namespace duskland::tui;
document::document() {
  set_selectable(true);
  set_rect({0, 0, (uint32_t)getmaxx(stdscr), (uint32_t)getmaxy(stdscr)});
  on_active();
}
bool document::on_input(const util::key &key) {
  if (key.name() == "<tab>") {
    if (!next_active()) {
      next_active();
    }
    return true;
  }
  return false;
}