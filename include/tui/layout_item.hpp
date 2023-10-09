#pragma once
#include "window.hpp"
#include <string>
namespace duskland::tui {
class layout_item : public core::object {
private:
  core::auto_release<window> _win;
  core::auto_release<layout_item> _first;
  core::auto_release<layout_item> _second;
  layout_item *_parent;
  uint32_t _split_size;
  bool _vertical;
  std::string _name;
  util::rect _rect;
  friend class layout;

public:
  void split(const std::string &name, const int32_t &size = 0);
  void vsplit(const std::string &name, const int32_t &size = 0);
  const std::string &get_name() const;
  core::auto_release<window> get_window();
  layout_item(const std::string &name, const core::auto_release<window> &win);
  ~layout_item() override;
  void referesh();
  bool relayout();
};
} // namespace duskland::tui