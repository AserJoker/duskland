#include "tui/document.hpp"
#include <fmt/xchar.h>
using namespace duskland::tui;
document::document() {
  auto &attr = get_attribute();
  attr.position = attribute::ABSOLUTE;
  attr.size.width = getmaxx(stdscr);
  attr.size.height = getmaxy(stdscr);
  attr.offset.x = 0;
  attr.offset.y = 0;
  request_update();
}
bool document::on_input(const util::key &key) {
  if (key.name == "<resize>") {
    auto &attr = get_attribute();
    attr.size.width = getmaxx(stdscr);
    attr.size.height = getmaxy(stdscr);
    request_update();
    return true;
  }
  return widget::on_input(key);
}
void document::on_render(core::auto_release<graphic> &g) {
  g->draw(0, 0,
          fmt::format(L"{},{}", get_attribute().size.width,
                      get_attribute().size.height));
}