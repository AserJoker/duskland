#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/border.hpp"
#include "util/config.hpp"
#include "util/rect.hpp"
#include "util/size.hpp"
#include "widget_base.hpp"
#include <ncurses.h>
#include <string>
namespace duskland::tui {
class system_tui;
class window : public widget_base {
private:
  WINDOW *_win;
  util::border _border;
  int _border_color;
  util::rect _content_rect;
  int32_t _current_line;
  int32_t _current_column;

  void fix_rect();

  friend class system_tui;

  void fix_content_rect();

protected:
  core::auto_release<system_tui> _tui;
  core::auto_release<util::config> _config;
  void draw_border();
  void draw_scroll();

public:
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
  void on_update() override;
  void on_active() override;
  void on_dective() override;
  window(const util::rect &rc, const std::string &name = "");
  ~window() override;
  void update();
  void set_border(const util::border &border);
  const util::border &get_border() const;
  void set_rect(const util::rect &rc);
  void resize(int32_t dw, int32_t dh);
  void move(int32_t x, int32_t y);
  void refresh();
  void active();
  void clear();
  void set_content_rect(const util::rect &content_rc);
  const util::rect &get_content_rect() const;
  void move_content(const int32_t &dx, const int32_t &dy);
  void write(const uint32_t &x, const uint32_t &y, const wchar_t &ch,
             const uint32_t &attr = 0);
  void write(const uint32_t &x, const uint32_t &y, const wchar_t *ch,
             const uint32_t &attr = 0);
  void write(const uint32_t &x, const uint32_t &y, const char &ch,
             const uint32_t &attr = 0);
  void write(const uint32_t &x, const uint32_t &y, const char *ch,
             const uint32_t &attr = 0);
  void set_current_pos(const int32_t &line, const int32_t &column);
  void enable_input(const int32_t &x, const int32_t &y);
  void disable_input();
};
} // namespace duskland::tui