#pragma once
#include "attribute.hpp"
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "graphic.hpp"
#include "util/key.hpp"
#include <vector>
namespace duskland::tui {
class widget : public core::object {
private:
  widget *_parent;
  widget *_active_widget;
  std::vector<core::auto_release<widget>> _children;
  util::rect _rect;
  attribute _attr;
  bool _is_changed;
  bool _update_lock;
  bool _is_active;
  util::rect _fixed_rect;

private:
  void draw_border(core::auto_release<graphic> &g);
  void draw_scroll(core::auto_release<graphic> &g);
  void calculate_pos();
  void calculate_width();
  void calculate_height();
  void calculate_fixed();
  void relayout();

protected:
  static widget *_root;

protected:
  virtual void on_render(core::auto_release<graphic> &g);
  virtual void on_update();
  virtual void on_active();
  virtual void on_dective();
  virtual void on_event(const std::string &event, widget *emitter);
  virtual bool on_input(const util::key &key);

public:
  widget();
  void clear(core::auto_release<graphic> &g);
  void emit(const std::string &event);
  std::vector<core::auto_release<widget>> &get_children();
  void render(core::auto_release<graphic> &g);
  widget *get_parent();
  util::rect get_bound_rect();
  util::point get_focus() const;
  const util::rect &get_rect() const;
  widget *get_active();
  void request_update();
  attribute &get_attribute();
  void add_child(const core::auto_release<widget> &w);
  void remove_child(const core::auto_release<widget> &w);
  void set_focus(const util::point &pos);
  bool next_active();
  bool last_active();
  bool is_active();
  void set_active(widget *w);
  bool process_input(const util::key &key);
};
} // namespace duskland::tui