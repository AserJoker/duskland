#include "../include/document.hpp"
#include "core/include/auto_release.hpp"
#include "tui/include/widget.hpp"
#include <curses.h>
using namespace duskland;
using namespace duskland::tui;
document::document() { set_size({getmaxx(stdscr), getmaxy(stdscr)}); }

bool document::process(const util::key &key) {
  if (key.name == "<resize>") {
    set_size({getmaxx(stdscr), getmaxy(stdscr)});
  }
  return widget::process(key);
};
void document::render(core::auto_release<graphic> &g) {
  g->set_position({0, 0});
  g->set_viewport({0, 0, 0, 0});
  widget::render(g);
}