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
  std::vector<core::auto_release<widget>> _children;
  util::rect _rect;
  attribute _attr;
  bool _is_changed;
  bool _update_lock;
  util::rect _fixed_rect;

private:
  void clear(core::auto_release<graphic> &g);
  void draw_border(core::auto_release<graphic> &g);
  void draw_scroll(core::auto_release<graphic> &g);
  void calculate_rect();
  void calculate_pos();
  void calculate_width();
  void calculate_height();
  void calculate_fixed();

protected:
  virtual void on_render(core::auto_release<graphic> &g);
  virtual void on_update();

public:
  widget();
  void emit(const std::string &event);
  widget *get_parent();
  std::vector<core::auto_release<widget>> &get_children();
  void render(core::auto_release<graphic> &g);
  util::rect get_bound_rect();
  void request_update();
  virtual void on_event(const std::string &event, widget *emitter);
  virtual bool on_input(const util::key &key);
  attribute &get_attribute();
  void add_child(const core::auto_release<widget> &w);
  void remove_child(const core::auto_release<widget> &w);
  void set_focus(const util::point &pos);
  util::point get_focus() const;
  const util::rect &get_rect() const;
};
} // namespace duskland::tui