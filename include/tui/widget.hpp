#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "graphic.hpp"
#include "util/border.hpp"
#include "util/key.hpp"
#include "util/rect.hpp"
#include <vector>
namespace duskland::tui {
class widget : public core::object {
private:
  std::vector<core::auto_release<widget>> _children;
  widget *_parent;
  widget *_active_widget;
  util::rect _rect;
  bool _is_selectable;
  bool _is_active;
  bool _is_changed;
  util::border_info _border;

protected:
  util::rect &get_rect();

protected:
  void draw_border(const core::auto_release<graphic> &g);
  void clear(const core::auto_release<graphic> &g);
  std::vector<core::auto_release<widget>> &get_children();
  widget *get_parent();

public:
  widget();
  const util::rect &get_rect() const;
  const util::rect get_content_rect() const;
  void render(const core::auto_release<graphic> &g);
  bool is_selectable();
  void set_selectable(bool selectable);
  bool is_active();
  void set_border(const util::border_info &border);
  const util::border_info &get_border() const;
  void force_update();
  void add_child(const core::auto_release<widget> &widget);
  void remove_child(const core::auto_release<widget> &widget);
  bool next_active();
  void set_active_widget(const core::auto_release<widget> &widget);
  void set_rect(const util::rect &rc);
  virtual bool on_input(const util::key &key);
  virtual void on_dective();
  virtual void on_active();
  virtual void on_render(const core::auto_release<graphic> &g);
};
} // namespace duskland::tui