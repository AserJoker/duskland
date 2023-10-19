#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "util/border.hpp"
#include "util/injector.hpp"
#include "util/key.hpp"
#include "util/rect.hpp"
#include <ncurses.h>
namespace duskland::tui {
class window : public core::object {
private:
  util::rect _rect;
  core::auto_release<util::injector> _injector;
  bool _is_active;
  WINDOW *_win;
  std::string _name;
  util::border_info _border;

  util::rect _content_rect;

  util::rect &get_rect();
  void fix_content_rect();
  util::rect get_bound_rect();
  void draw_border_node(const int32_t &x, const int32_t &y);

protected:
  void draw_scroll();
  void draw_border();

public:
  window(const std::string &name);
  ~window();
  void initialize(const util::rect &rect);
  void render();
  void refresh();
  void dispose();
  void active();
  void clear();
  bool is_active();
  void move_to(const int32_t &x, const int32_t &y);
  void resize_to(const uint32_t &width, const uint32_t &height);
  void move(const int32_t &dx, const int32_t &dy);
  void resize(const int32_t &dw, const int32_t &dh);

  const util::border_info &get_border_info() const;
  void set_border_info(const util::border_info &info);

  void draw(const int32_t &x, const int32_t &y, const wchar_t &ch,
            const chtype &attr);
  void draw(const int32_t &x, const int32_t &y, const char &ch,
            const chtype &attr);
  void draw(const int32_t &x, const int32_t &y, const std::wstring &str,
            const chtype &attr);
  void draw(const int32_t &x, const int32_t &y, const std::string &str,
            const chtype &attr);

  void draw_absolute(const int32_t &x, const int32_t &y, const wchar_t &ch,
                     const chtype &attr);
  void draw_absolute(const int32_t &x, const int32_t &y, const char &ch,
                     const chtype &attr);
  void draw_absolute(const int32_t &x, const int32_t &y,
                     const std::wstring &str, const chtype &attr);
  void draw_absolute(const int32_t &x, const int32_t &y, const std::string &str,
                     const chtype &attr);
  const std::string &get_name() const;
  const util::rect &get_rect() const;
  const util::rect &get_content_rect() const;
  void set_content_rect(const util::rect &rc);
  void set_current_pos(const int32_t &x, const int32_t &y);

  virtual void on_initialize();
  virtual void on_active();
  virtual void on_dective();
  virtual bool on_command(const util::command &cmd);
  virtual void on_render();
  virtual void on_dispose();
  virtual void on_resize();
};
}; // namespace duskland::tui