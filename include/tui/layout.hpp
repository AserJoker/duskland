#pragma once
#include "core/auto_release.hpp"
#include "core/object.hpp"
#include "core/singleton.hpp"
#include "layout_item.hpp"
#include "system_tui.hpp"
namespace duskland::tui {
class layout : public core::object {
private:
  core::auto_release<system_tui> _tui;
  core::auto_release<layout_item> _root;

public:
  layout();
  ~layout() override;
  void initialize();
  void uninitialize();
  core::auto_release<layout_item> get_active_item();
  bool relayout();
  core::auto_release<layout_item> select(const std::string &name) const;
  void remove(const core::auto_release<layout_item>& item);
};
}; // namespace duskland::tui