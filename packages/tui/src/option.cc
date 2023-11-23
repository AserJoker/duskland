#include "../include/option.hpp"
#include "util/include/xchar.hpp"
namespace duskland::tui {
option::option(const util::option &opt) : _is_selected(false), _opt(opt) {
  auto &attr = get_attribute();
  attr.size.height = 1;
  attr.size.width =
      util::wcswidth(opt.display_name.c_str(), opt.display_name.length()) + 1;
  request_update();
}
bool option::on_input(const util::key &key) {
  if (key.name == "<enter>") {
    emit("select");
    return true;
  }
  return widget::on_input(key);
}
void option::set_selected(bool selected) {
  _is_selected = selected;
  request_update();
}
bool option::is_selected() const { return _is_selected; }
void option::on_render(core::auto_release<graphic> &g) {
  if (_is_selected) {
    g->draw(0, 0, g->get_brush()->query_symbol("select.option"));
  }
  g->draw(1, 0, _opt.display_name.c_str());
}

const std::wstring &option::get_value() const { return _opt.value; }
} // namespace duskland::tui