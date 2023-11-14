#pragma once
#include "core/object.hpp"
#include "core/singleton.hpp"
#include "input.hpp"
#include "resource.hpp"
#include "tui/graphic.hpp"
#include "tui/widget.hpp"
#include "util/key.hpp"
#include <string>
#include <vector>
namespace duskland::system {
class application : public core::object {

private:
  bool _is_running;
  core::auto_release<tui::graphic> _graphic;
  core::auto_release<input> _input;
  core::auto_release<resource> _resource;
  core::auto_release<util::color> _colors;
  core::auto_release<tui::widget> _root;

public:
  int run();
  application();
  ~application() override;
  void initialize(int argc, char *argv[]);
  void exit();
  void on_command(const util::key &cmd);
};
} // namespace duskland::system