#include "../include/document.hpp"
using namespace duskland::tui;
document::document() {
  auto &attr = get_attribute();
  attr.position = style::ABSOLUTE;
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
  return widget::on_input(key);
}