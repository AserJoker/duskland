#include "tui/widget_base.hpp"
using namespace duskland::tui;
widget_base::widget_base(const std::string &name) : widget(name) {}
void widget_base::render(const core::auto_release<window> &win) {}