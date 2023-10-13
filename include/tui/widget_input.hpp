﻿#pragma once
#include "widget.hpp"
namespace duskland::tui {
class widget_input : public widget {
private:
  uint32_t _max_length;
  std::wstring _input;
  int _mode;

public:
  widget_input(const std::string &name, const uint32_t &max_length);
  void render(const core::auto_release<window> &win,
              const util::position &pos) override;
  bool on_command(int cmd,
                  const core::auto_release<widget_base> &emitter) override;
  const std::wstring &get_input() const;
};
} // namespace duskland::tui