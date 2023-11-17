#pragma once
#include "tui/input.hpp"
#include "widget.hpp"
#include <codecvt>
namespace duskland::tui {
class document : public widget {
protected:
  bool on_input(const util::key &key) override;

  std::wstring name;
  std::wstring val;

public:
  document();
  void on_event(const std::string &event, widget *emitter) override {
    if (event == "change") {
      input *i = (input *)emitter;
      name = i->get_name();
      val = i->get_value();
      request_update();
    }
  }
  void on_render(core::auto_release<graphic> &g) override {
    g->draw(10, 10, val);
    g->draw(10, 11, name);
  }
};
} // namespace duskland::tui