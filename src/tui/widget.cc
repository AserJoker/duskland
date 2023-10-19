#include "tui/widget.hpp"
#include "core/singleton.hpp"
using namespace duskland::tui;
using namespace duskland;
widget::widget(const std::string &name)
    : _name(name), _rect({0, 0, 0, 0}), _is_active(false) {
  _injector = core::singleton<util::injector>::get();
}
void widget::render(const core::auto_release<window> &win) {}
void widget::set_rect(const util::rect &rc) { _rect = rc; }
const util::rect &widget::get_rect() const { return _rect; }
bool widget::is_active() const { return _is_active; }
void widget::on_active() { _is_active = true; }
void widget::on_dective() { _is_active = false; }
bool widget::on_command(const core::auto_release<window> &emitter,
                        const util::command &cmd) {
  return false;
}
void widget::set_tab_index(const int32_t &tab_index) {
  _tab_index = tab_index;
};
const int32_t &widget::get_tab_index() { return _tab_index; }