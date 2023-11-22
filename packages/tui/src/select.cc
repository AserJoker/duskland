#include "../include/select.hpp"
#include "util/include/xchar.hpp"
using namespace duskland::tui;
using namespace duskland;
class selector : public widget {
private:
  std::vector<util::option> _options;
  std::wstring _value;

public:
  selector(const std::vector<util::option> &options, const std::wstring &val)
      : _options(options), _value(val) {
    auto &attr = get_attribute();
    attr.position = style::ABSOLUTE;
    attr.border_left = true;
    attr.border_right = true;
    attr.border_bottom = true;
    attr.border_top = true;
    attr.size.width = 0;
    attr.size.height = options.size();
    for (auto &opt : options) {
      if (attr.size.width <
          util::wcswidth(opt.display_name.c_str(), opt.display_name.length())) {
        attr.size.width =
            util::wcswidth(opt.display_name.c_str(), opt.display_name.length());
      }
    }
    if (attr.size.width < 10) {
      attr.size.width = 10;
    }
    wint_t width, height;
    getmaxyx(stdscr, height, width);
    attr.offset.x = (width - attr.size.width) / 2;
    attr.offset.y = (height - attr.size.height) / 2;
    if (_value.empty()) {
      _value = options[0].value;
    }
    request_update();
  }
  bool on_input(const util::key &key) override {
    if (key.name == "<enter>") {
      auto s = (class select *)get_parent();
      s->set_value(_value);
      get_parent()->remove_child(this);
      return true;
    }
    if (key.name == "<tab>") {
      auto it = _options.begin();
      for (; it != _options.end(); it++) {
        if (it->value == _value) {
          break;
        }
      }
      it++;
      if (it == _options.end()) {
        it = _options.begin();
      }
      _value = it->value;
      return true;
    }
    if (key.name == "<esc>") {
      get_parent()->remove_child(this);
      return true;
    }
    return widget::on_input(key);
  }
  void on_render(core::auto_release<graphic> &g) override {
    auto y = 0;
    for (auto &opt : _options) {
      if (opt.value == _value) {
        g->set_attr("focus");
      } else {
        g->set_attr("normal");
      }
      g->draw(0, y, opt.display_name);
      y++;
    }
  }
};
select::select(const std::wstring &name,
               const std::vector<util::option> &options)
    : _options(options), component(name) {
  auto &attr = get_attribute();
  if (!attr.size.width) {
    for (auto &opt : options) {
      if (attr.size.width <
          util::wcswidth(opt.display_name.c_str(), opt.display_name.length())) {
        attr.size.width =
            util::wcswidth(opt.display_name.c_str(), opt.display_name.length());
      }
    }
    if (options.empty()) {
      attr.size.width = 8;
    }
    attr.size.width += 4;
  }
  request_update();
}
void select::on_render(core::auto_release<graphic> &g) {
  g->draw(0, 0, L'[');
  std::wstring str;
  for (auto &opt : _options) {
    if (opt.value == get_value()) {
      str = opt.display_name;
    }
  }
  while (util::wcswidth(str.c_str(), str.length()) < get_rect().width - 4) {
    str += L'_';
  }
  g->draw(2, 0, str);
  g->draw(get_rect().width - 1, 0, L']');
}
bool select::on_input(const util::key &key) {
  if (key.name == "<enter>") {
    if (get_children().empty()) {
      if (!_options.empty()) {
        auto win = new selector(_options, get_value());
        add_child(win);
        set_active(win);
      }
      return true;
    }
  }
  if (key.name == "<delete>") {
    set_value(L"");
    request_update();
    return true;
  }
  return widget::on_input(key);
}