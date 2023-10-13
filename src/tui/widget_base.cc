#include "tui/widget_base.hpp"
using namespace duskland::tui;
using namespace duskland;
widget_base::widget_base(const std::string &name)
    : _is_active(false), _name(name), _rect({0, 0, 0, 0}), _select_index(0) {}
const util::rect &widget_base::get_rect() const { return _rect; }
util::rect &widget_base::get_rect() { return _rect; }
const std::string &widget_base::get_name() const { return _name; }
void widget_base::set_name(const std::string &name) { _name = name; }
bool widget_base::is_active() { return _is_active; }
void widget_base::on_active() {
  _is_active = true; }
void widget_base::on_dective() { _is_active = false; }
void widget_base::on_update() {}
bool widget_base::on_command(int ch,
                             const core::auto_release<widget_base> &emitter) {
  return false;
}

int widget_base::get_select_index() { return _select_index; };
void widget_base::set_select_index(int value) { _select_index = value; };