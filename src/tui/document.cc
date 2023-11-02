#include "tui/document.hpp"
using namespace duskland::tui;
document::document() {
  set_selectable(true);
  set_rect({0, 0, (uint32_t)getmaxx(stdscr), (uint32_t)getmaxy(stdscr)});
  on_active();
}
bool document::on_input(const util::key &key) {
  if (!widget::on_input(key)) {
    if (key.name() == "<tab>") {
      if (!next_active()) {
        next_active();
      }
      return true;
    }
  }
  return false;
}
void document::on_render(const core::auto_release<graphic> &g) {
  g->set_view_port(get_content_rect());
  g->set_position({0,0});
}