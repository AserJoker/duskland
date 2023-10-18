#pragma once
#include "core/object.hpp"
#include "window.hpp"
#include <ncurses.h>
#include <vector>
namespace duskland::tui {
class system_wm : public core::object {
private:
  window *_active_window;
  std::vector<window *> _windows;
  std::vector<util::layout_info> _layout_info;

public:
  system_wm();
  bool on_command(const wint_t &cmd);
  void set_active_window(window *win);
  window *get_active_window();
  void initialize();
  void uninitialize();
  std::vector<window *> &get_windows();
  void next_active();
  void refresh();

  void set_access_node(const int32_t &x, const int32_t &y,
                       const util::border_info &border);
  const util::layout_info *get_access_node(const int32_t &x, const int32_t &y);
  void remove_access_node(const int32_t &x, const int32_t &y,
                          const util::border_info &border);

  window *select(const std::string &name);
};
}; // namespace duskland::tui