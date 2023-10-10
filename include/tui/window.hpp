#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/attriabute.hpp"
#include "util/border.hpp"
#include "util/rect.hpp"
#include "util/size.hpp"
#include "widget.hpp"
#include <ncurses.h>
#include <string>
namespace duskland::tui {
class system_tui;
class window : public widget {
private:
  core::auto_release<system_tui> _tui;
  core::auto_release<util::attribute> _attribute;
  WINDOW *_win;
  util::border_style _border_style;
  util::border _border;
  int _border_color;

  void fix_rect();

  friend class system_tui;

protected:
  void draw_border();

public:
  void on_command(int cmd) override;
  void on_update() override;
  void on_active() override;
  void on_dective() override;
  window(const util::rect &rc, const std::string &name = "");
  ~window() override;
  void update();
  void set_border(const util::border &border);
  void set_border_style(const util::border_style &style);
  const util::border &get_border() const;
  void set_rect(const util::rect &rc);
  void resize(int32_t dw, int32_t dh);
  void move(int32_t x, int32_t y);
  void refresh();
  void active();
  void clear();
  const util::rect get_client_rect() const;
  void write(const uint32_t &x, const uint32_t &y, const wchar_t &ch,
             const uint32_t &attr = 0);
  void write(const uint32_t &x, const uint32_t &y, const wchar_t *ch,
             const uint32_t &attr = 0);
};
} // namespace duskland::tui