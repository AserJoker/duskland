#pragma once
#include "core/object.hpp"
#include "core/singleton.hpp"
#include "tui/system_wm.hpp"
#include "util/injector.hpp"
#include <string>
#include <vector>
namespace duskland::system {
class application : public core::object {
public:
  enum cursor_style { CUR_INVISIBLE = 0, CUR_NORMAL = 1, CUR_VERY_VISIBLE = 2 };

private:
  bool _is_running;
  core::auto_release<util::injector> _injector;
  core::auto_release<tui::system_wm> _wm;
  core::auto_release<tui::window> _win;

private:
  void read_command();
  void decode_command(const std::vector<wint_t> &codes);

public:
  int run();
  application();
  ~application() override;
  void initialize(int argc, char *argv[]);
  void exit();
  void set_cursor_style(cursor_style style);
  void clear();
  void command(const util::key &cmd);
};
} // namespace duskland::system