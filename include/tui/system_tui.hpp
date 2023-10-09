#pragma once
#include "core/object.hpp"
#include "window.hpp"
#include <vector>
namespace duskland::tui {
class system_tui : public core::object {
private:
  std::vector<window *> _windows;
  window *_active_window;

public:
  system_tui();
  void initialize();
  void uninitialize();
  void run_command(int ch);
  window *get_active_window();
  void set_active_window(window *);
  void add_window(window *win);
  void remove_window(window *win);
  void refresh();
  void next_active();
  void last_active();
};
} // namespace duskland::tui