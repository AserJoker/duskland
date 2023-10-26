#pragma once
#include "core/object.hpp"
#include "util/key.hpp"
#include "util/size.hpp"
#include "window.hpp"
namespace duskland::tui {
class widget : public core::object {
private:
  util::rect _rect;
  std::string _name;
  bool _is_active;
  int32_t _tab_index;

protected:
  core::auto_release<util::injector> _injector;

public:
  const util::rect &get_rect() const;
  void set_rect(const util::rect &rc);
  widget(const std::string &name);
  virtual void render(const core::auto_release<window> &win);
  bool is_active() const;
  void set_tab_index(const int32_t &tab_index);
  const int32_t &get_tab_index();

  virtual void on_active();
  virtual void on_dective();
  virtual bool on_command(const core::auto_release<window> &emitter,
                          const util::key &cmd);
};
} // namespace duskland::tui