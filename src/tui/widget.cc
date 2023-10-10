#include "tui/widget.hpp"
using namespace duskland::tui;
using namespace duskland;
widget::widget(const std::string &name)
    : _is_active(false), _name(name), _rect({0, 0, 0, 0}) {}
const util::rect &widget::get_rect() const { return _rect; }
util::rect &widget::get_rect() { return _rect; }
const std::string &widget::get_name() const { return _name; }
void widget::set_name(const std::string &name) { _name = name; }
bool widget::is_active() { return _is_active; }
void widget::on_active() { _is_active = true; }
void widget::on_dective() { _is_active = false; }
void widget::on_update() {}
void widget::on_command(int ch) {}