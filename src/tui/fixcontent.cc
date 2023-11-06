#include "tui/fixcontent.hpp"
#include "util/injector.hpp"
#include <curses.h>
#include <fmt/xchar.h>
using namespace duskland::tui;
using namespace duskland;
fixcontent::fixcontent() : _max_size({0, 0}) {
  set_selectable(true);
  set_hidden_overflow(true);
  set_border({true, true, true, true});
}
void fixcontent::fix_rect() {
  auto offset = get_offset();
  auto &rc = get_content_rect();
  if (_focus.x + offset.x < 0) {
    offset.x = -_focus.x;
  }
  if (_focus.y + offset.y < 0) {
    offset.y = -_focus.y;
  }
  if (_focus.y + offset.y >= rc.height) {
    offset.y = rc.height - _focus.y - 1;
  }
  if (_focus.x + offset.x >= rc.width) {
    offset.x = rc.width - _focus.x - 1;
  }
  if (offset.x > 0) {
    offset.x = 0;
  }
  if (offset.y > 0) {
    offset.y = 0;
  }
  if (_max_size.height <= get_content_rect().height) {
    offset.y = 0;
  } else {
    if (-offset.y > (_max_size.height - get_content_rect().height)) {
      offset.y = -(_max_size.height - get_content_rect().height);
    }
  }
  if (_max_size.width < get_content_rect().width) {
    offset.x = 0;
  } else {
    if (-offset.x > (_max_size.width - get_content_rect().width)) {
      offset.x = -(_max_size.width - get_content_rect().width);
    }
  }
  set_offset(offset);
}
void fixcontent::on_update() {
  _max_size.width = 0;
  _max_size.height = 0;
  for (auto &c : get_children()) {
    auto &rc = c->get_rect();
    if (rc.width > _max_size.width) {
      _max_size.width = rc.width;
    }
    if (rc.height > _max_size.height) {
      _max_size.height = rc.height;
    }
  }
  fix_rect();
}
void fixcontent::draw_scroll(const core::auto_release<graphic> &g) {
  auto &border = get_border();
  auto &rc = get_content_rect();
  auto &offset = get_offset();
  if (border.bottom) {
    std::wstring str;
    if (_max_size.height > rc.height) {
      str += fmt::format(
          L"{}% column",
          (uint32_t)(offset.y * 1.0f /
                     ((int32_t)rc.height - (int32_t)_max_size.height) * 100));
    }
    if (_max_size.width > rc.width) {
      auto line = fmt::format(
          L"{}% line",
          (uint32_t)(offset.x * 1.0f /
                     ((int32_t)rc.width - (int32_t)_max_size.width) * 100));
      if (str.empty()) {
        str = line;
      } else {
        str = fmt::format(L"{} {}", str, line);
      }
    }
    g->draw(get_rect().x + 1, get_rect().y + get_rect().height - 1,
            str.c_str());
  }
}
void fixcontent::on_render(const core::auto_release<graphic> &g) {
  draw_scroll(g);
}

void fixcontent::set_position(const util::position &pos) {
  set_offset(pos);
  fix_rect();
}
const util::position &fixcontent::get_position() const { return get_offset(); };
void fixcontent::on_event(const std::string &event, widget *w) {
  if (event == "focus") {
    auto wrc = w->get_absolute_rect();
    auto crc = get_content_rect();
    _focus.y = wrc.y - crc.y;
    _focus.x = wrc.x - crc.x;
    fix_rect();
  } else {
    widget::on_event(event, w);
  }
}