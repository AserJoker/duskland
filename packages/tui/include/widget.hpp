#pragma once
#include "core/include/auto_release.hpp"
#include "core/include/object.hpp"
#include "graphic.hpp"
#include "util/include/key.hpp"
#include "util/include/point.hpp"
#include "util/include/size.hpp"
#include <chrono>
#include <vector>
namespace duskland::tui {
class widget : public core::object {

private:
  widget *_parent;
  std::vector<core::auto_release<widget>> _children;

  widget *_active;

  util::size _size;
  util::point _offset;

protected:
  virtual void on(const std::string &event, widget *emitter);

public:
  widget();
  virtual void render(core::auto_release<graphic> &g);
  virtual void relayout();
  virtual bool process(const util::key& cmd);

  void add_child(const core::auto_release<widget> &child);
  void remove_child(const core::auto_release<widget> &child);
  const std::vector<core::auto_release<widget>> &get_children() const;
  widget *get_parent();

  const util::size &get_size() const;
  const util::point &get_offset() const;
  void set_size(const util::size &size);
  void set_offset(const util::point &offset);

  void emit(const std::string &event);
};
} // namespace duskland::tui