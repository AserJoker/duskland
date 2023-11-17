#include "tui/list_select.hpp"
using namespace duskland::tui;
list_select::list_select(const std::wstring &name,
                         const std::vector<util::option> &options)
    : _options(options), component<std::wstring>(name), _is_selecting(false) {
  auto &attr = get_attribute();
  attr.size.width = 0;
  attr.size.height = options.size();
  for (auto &opt : options) {
    if (attr.size.width - 3 <
        wcswidth(opt.display_name.c_str(), opt.display_name.length())) {
      attr.size.width =
          wcswidth(opt.display_name.c_str(), opt.display_name.length()) + 3;
    }
  }
  if (!options.empty()) {
    _current = options[0].value;
  }
  request_update();
}
void list_select::on_active() {
  component::on_active();
  _is_selecting = true;
  request_update();
}
void list_select::on_dective() {
  component::on_dective();
  _is_selecting = false;
  request_update();
}
void list_select::on_render(core::auto_release<graphic> &g) {
  auto y = 0;
  for (auto &opt : _options) {
    if (opt.value == _current && is_active()) {
      g->set_attr("active");
    } else {

      g->set_attr("normal");
    }
    if (opt.value == _current && _is_selecting) {
      g->draw(0, y, L'[');
    }
    g->draw(2, y, opt.display_name);
    if (opt.value == get_value()) {
      g->draw(1, y, L'*');
    }
    if (opt.value == _current && _is_selecting) {
      g->draw(get_rect().width - 1, y, L']');
    }
    y++;
  }
}
bool list_select::on_input(const util::key &key) {
  if (_is_selecting) {
    if (key.name == "<enter>") {
      set_value(_current);
      _is_selecting = false;
      return true;
    }
    if (key.name == "<tab>") {
      auto it = _options.begin();
      for (; it != _options.end(); it++) {
        if (it->value == _current) {
          break;
        }
      }
      it++;
      if (it == _options.end()) {
        it = _options.begin();
      }
      _current = it->value;
      request_update();
      return true;
    }
    if (key.name == "<s-tab>") {
      auto it = _options.rbegin();
      for (; it != _options.rend(); it++) {
        if (it->value == _current) {
          break;
        }
      }
      it++;
      if (it == _options.rend()) {
        it = _options.rbegin();
      }
      _current = it->value;
      request_update();
      return true;
    }
  } else {
    if (key.name == "<enter>") {
      _is_selecting = true;
      request_update();
      return true;
    }
  }
  return component::on_input(key);
}