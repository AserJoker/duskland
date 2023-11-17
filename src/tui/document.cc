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
  widget::_root = this;
}
bool document::on_input(const util::key &key) {
  if (key.name == "<resize>") {
    auto &attr = get_attribute();
    attr.size.width = getmaxx(stdscr);
    attr.size.height = getmaxy(stdscr);
    request_update();
    return true;
  }
  if (get_children().size()) {
    if (get_active()) {
      if (get_active()->on_input(key)) {
        return true;
      }
    }
    if (key.name == "<tab>") {
      if (!next_active()) {
        next_active();
      }
      return true;
    }
  }
  return false;
}