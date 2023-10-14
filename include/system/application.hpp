﻿#pragma once
#include "core/object.hpp"
#include "core/singleton.hpp"
#include "system_input.hpp"
#include "tui/layout.hpp"
#include "tui/system_tui.hpp"
#include "util/config.hpp"
#include <string>
#include <vector>
namespace duskland::system {
class application : public core::object {
public:
  enum cursor_style { CUR_INVISIBLE = 0, CUR_NORMAL = 1, CUR_VERY_VISIBLE = 2 };

private:
  bool _is_running;
  std::vector<std::string> _args;
  core::auto_release<tui::system_tui> _tui;
  core::auto_release<tui::layout> _layout;
  core::auto_release<util::config> _config;
  core::auto_release<system_input> _input;

public:
  int run();
  application();
  ~application() override;
  void initialize(int argc, char *argv[]);
  void exit();
  const std::vector<std::string> &argv() const;
  void set_cursor_style(cursor_style style);
  void clear();
  void command(wint_t cmd);
};
} // namespace duskland::system