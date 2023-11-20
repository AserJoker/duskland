#include "tui/list_select.hpp"
#include "tui/layout_vertical.hpp"
#include "tui/text.hpp"
using namespace duskland::tui;
list_select::list_select(const std::wstring &name,
                         const std::vector<util::option> &options)
    : component<std::wstring>(name), _is_selecting(false), _current(nullptr) {
  auto layout = new layout_vertical();
  add_child(layout);
  set_options(options);
  request_update();
}
bool list_select::on_input(const util::key &key) {
  if (key.name == "<delete>") {
    if (_current && _current->is_active()) {
      _current->set_selected(false);
      _current = nullptr;
    }
  }
  return component::on_input(key);
}
void list_select::on_event(const std::string &event, widget *emitter) {
  if (event == "select") {
    if (_current) {
      _current->set_selected(false);
    }
    _current = (tui::option *)emitter;
    _current->set_selected(true);
    set_value(_current->get_value());
  }
  widget::on_event(event, emitter);
}

void list_select::set_options(const std::vector<util::option> &opt) {
  _options = opt;
  auto layout = get_children()[0];
  if (_current) {
    _current->set_selected(false);
  }
  layout->get_children().clear();
  for (auto &opt : _options) {
    layout->add_child(new option(opt));
  }
  _current = nullptr;
  set_value(L"");
}