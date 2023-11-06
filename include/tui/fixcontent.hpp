#pragma once
#include "util/position.hpp"
#include "util/size.hpp"
#include "widget.hpp"
namespace duskland::tui {
class fixcontent : public widget {
private:
  util::size _max_size;
  util::position _focus;
  void fix_rect();
  void draw_scroll(const core::auto_release<graphic> &g);

public:
  fixcontent();
  virtual void on_update() override;
  virtual void on_render(const core::auto_release<graphic> &g) override;
  void set_position(const util::position &pos);
  const util::position &get_position() const;
  void on_event(const std::string &event, widget *w) override;
};
}; // namespace duskland::tui