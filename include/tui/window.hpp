#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/border.hpp"
#include "util/rect.hpp"
#include "util/size.hpp"
#include <ncurses.h>
#include <string>
namespace duskland::tui {
class system_tui;
class window : public core::object {
private:
  util::rect _rect;
  util::border_style _border_style;
  util::border _border;
  int _border_color;
  WINDOW *_win;
  core::auto_release<system_tui> _tui;
  bool _is_active;
  std::string _name;

  void fix_rect();

  friend class system_tui;

protected:
  virtual void on_active();
  virtual void on_dective();
  virtual void on_command(int cmd);
  virtual void on_update();
  void draw_border();

public:
  window(const util::rect &rc, const std::string &name = "");
  ~window() override;
  void update();
  void set_border(const util::border &border);
  const util::border &get_border() const;
  const util::rect &get_rect() const;
  void set_rect(const util::rect &rc);
  void resize(int32_t dw, int32_t dh);
  void move(int32_t x, int32_t y);
  void set_border_color(int index);
  void refresh();
  void active();
  bool is_active();
  void clear();
  const util::rect get_client_rect() const;
  const std::string &get_name() const;
  void set_name(const std::string &name);
};
} // namespace duskland::tui