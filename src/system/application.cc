#include "system/application.hpp"
#include "tui/widget_col.hpp"
#include "tui/widget_input.hpp"
#include "tui/widget_line.hpp"
#include "tui/widget_text.hpp"
#include "tui/window_widget.hpp"
#include "util/event.hpp"
#include <chrono>
#include <codecvt>
#include <fmt/format.h>
#include <fmt/xchar.h>
#include <iostream>
#include <ncurses.h>
#include <stdexcept>
#include <thread>
using namespace duskland::system;
using namespace duskland;
class window_demo : public tui::window_widget {
public:
  void on_initialize() override {
    tui::window_widget::on_initialize();
    auto line = new tui::widget_line("line");
    auto text1 = new tui::widget_text("label", L"输入测试:");
    auto input = new tui::widget_input("input", 12);
    auto text2 = new tui::widget_text("tail", L"中文Tail");
    line->add_widget(text1);
    line->add_widget(input);
    line->add_widget(text2);
    line->next_active();
    get_root() = line;
    render();
  }
  window_demo() : tui::window_widget("demo window") {}
};
application::application() : _is_running(false) {
  _injector = core::singleton<util::injector>::get();
  _wm = core::singleton<tui::system_wm>::get();
  _win = new window_demo();
}
application::~application() {
  clrtoeol();
  refresh();
  endwin();
}
int application::run() {
  try {
    _is_running = true;
    while (_is_running) {
      read_command();
    }
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  _wm->uninitialize();
  return 0;
}
void application::exit() { _is_running = false; }

void application::initialize(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  raw();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  set_escdelay(1);
  this->clear();
  this->set_cursor_style(CUR_INVISIBLE);
  refresh();
  _injector->initialize();

  for (auto index = 0; index < argc; index++) {
    _injector->args().push_back(argv[index]);
  }
  _injector->attr("tui.border.normal", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.scroll.normal", COLOR_BLUE, COLOR_BLACK);
  _injector->attr("tui.text.normal", COLOR_WHITE, COLOR_BLACK, WA_DIM);
  _injector->attr("tui.text.focus", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.input.normal", COLOR_WHITE, COLOR_BLACK,
                  WA_NORMAL | WA_DIM);
  _injector->attr("tui.input.focus", COLOR_WHITE, COLOR_BLACK);
  _injector->attr("tui.input.cursor", COLOR_WHITE, COLOR_BLACK, WA_STANDOUT);

  _injector->keymap("key.next", '\t');
  _injector->keymap("key.select", '\n');

  _injector->keymap("key.quit", 'q');

  _injector->style("style.border.ls", L'│');
  _injector->style("style.border.rs", L'│');
  _injector->style("style.border.ts", L'─');
  _injector->style("style.border.bs", L'─');
  _injector->style("style.border.rb", L'┌');
  _injector->style("style.border.lb", L'┐');
  _injector->style("style.border.tr", L'└');
  _injector->style("style.border.tl", L'┘');
  _injector->style("style.border.tlr", L'┬');
  _injector->style("style.border.blr", L'┴');
  _injector->style("style.border.ltb", L'├');
  _injector->style("style.border.rtb", L'┤');
  _injector->style("style.border.lrtb", L'┼');

  _wm->initialize();
  _win->initialize(
      {0, 0, (uint32_t)getmaxx(stdscr), (uint32_t)getmaxy(stdscr)});
  _win->active();
  _win->render();
}
void application::set_cursor_style(cursor_style style) { curs_set(style); }
void application::clear() { ::clear(); }
void application::command(const util::key &cmd) {
  if (cmd.raw.empty()) {
    return;
  } else if (cmd.decode == _injector->keymap("key.quit")) {
    exit();
  } else {
    _wm->on_command(cmd);
  }
}
void application::read_command() {
  std::vector<wint_t> codes;
  for (;;) {
    wint_t c;
    if (_injector->feature("feature.text_input")) {
      if (get_wch(&c) == ERR) {
        c = ERR;
      }
    } else {
      c = getch();
    }
    if (c == ERR) {
      break;
    }
    codes.push_back(c);
  }
  decode_command(codes);
}
void application::decode_command(const std::vector<wint_t> &codes) {
  util::key cmd = {0, false, false, false, codes};
  const util::key_encoding keymap = {
      {{0x1b,
        {{
            {0x5b,
             {{
                 {0x44, {KEY_LEFT}},
                 {0x43, {KEY_RIGHT}},
                 {0x42, {KEY_DOWN}},
                 {0x41, {KEY_UP}},
                 {0x46, {KEY_END}},
                 {0x48, {KEY_HOME}},
                 {0x5a, util::key{'\t', true}},
                 {0x31,
                  {{{0x35,
                     {{{0x7e, {KEY_F(5)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(5), true}}}}},
                            {0x35,
                             {{{0x7e, util::key{KEY_F(5), false, true}}}}},
                        }}}}}},
                    {0x37,
                     {{{0x7e, {KEY_F(6)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(6), true}}}}},
                            {0x35,
                             {{{0x7e, util::key{KEY_F(6), false, true}}}}},
                        }}}}}},
                    {0x38,
                     {{{0x7e, {KEY_F(7)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(7), true}}}}},
                            {0x35,
                             {{{0x7e, util::key{KEY_F(7), false, true}}}}},
                        }}}}}},
                    {0x39,
                     {{{0x7e, {KEY_F(8)}},
                       {0x3b,
                        {{
                            {0x32, {{{0x7e, util::key{KEY_F(8), true}}}}},
                            {0x35,
                             {{{0x7e, util::key{KEY_F(8), false, true}}}}},
                        }}}}}},
                    {0x3b,
                     {{{0x32,
                        {{
                            {0x44, util::key{KEY_LEFT, true}},
                            {0x43, util::key{KEY_RIGHT, true}},
                            {0x42, util::key{KEY_DOWN, true}},
                            {0x41, util::key{KEY_UP, true}},
                            {0x46, util::key{KEY_END, true}},
                            {0x48, util::key{KEY_HOME, true}},
                            {0x50, util::key{KEY_F(1), true}},
                            {0x51, util::key{KEY_F(2), true}},
                            {0x52, util::key{KEY_F(3), true}},
                            {0x53, util::key{KEY_F(4), true}},
                        }}},
                       {0x35,
                        {{
                            {0x44, util::key{KEY_LEFT, false, true}},
                            {0x43, util::key{KEY_RIGHT, false, true}},
                            {0x42, util::key{KEY_DOWN, false, true}},
                            {0x41, util::key{KEY_UP, false, true}},
                            {0x46, util::key{KEY_END, false, true}},
                            {0x48, util::key{KEY_HOME, false, true}},
                            {0x50, util::key{KEY_F(1), false, true}},
                            {0x51, util::key{KEY_F(2), false, true}},
                            {0x52, util::key{KEY_F(3), false, true}},
                            {0x53, util::key{KEY_F(4), false, true}},
                        }}}}}}}}},
                 {0x32,
                  {{
                      {0x30,
                       {{{0x3b,
                          {{
                              {0x32, {{{0x7e, util::key{KEY_F(9), true}}}}},
                              {0x35,
                               {{{0x7e, util::key{KEY_F(9), false, true}}}}},
                          }}}}}},
                      {0x7e, util::key{KEY_IC}},
                      {0x3b,
                       {{
                           {0x32, {{{0x7e, util::key{KEY_IC, true}}}}},
                           {0x35, {{{0x7e, util::key{KEY_IC, false, true}}}}},
                       }}},
                      {0x30, {{{0x7e, {KEY_F(9)}}}}},
                      {0x31,
                       {{{0x7e, {KEY_F(10)}},
                         {0x3b,
                          {{
                              {0x32, {{{0x7e, util::key{KEY_F(10), true}}}}},
                              {0x35,
                               {{{0x7e, util::key{KEY_F(10), false, true}}}}},
                          }}}}}},
                      {0x33,
                       {{{0x7e, {KEY_F(11)}},
                         {0x3b,
                          {{
                              {0x32, {{{0x7e, util::key{KEY_F(11), true}}}}},
                              {0x35,
                               {{{0x7e, util::key{KEY_F(11), false, true}}}}},
                          }}}}}},
                      {0x34,
                       {{{0x7e, {KEY_F(12)}},
                         {0x3b,
                          {{
                              {0x32, {{{0x7e, util::key{KEY_F(12), true}}}}},
                              {0x35,
                               {{{0x7e, util::key{KEY_F(12), false, true}}}}},
                          }}}}}},
                  }}},
                 {0x33,
                  {{{0x7e, util::key{KEY_DC}},
                    {0x3b,
                     {{
                         {0x32, {{{0x7e, util::key{KEY_DC, true}}}}},
                         {0x35, {{{0x7e, util::key{KEY_DC, false, true}}}}},
                     }}}}}},
                 {0x35,
                  {{{0x7e, util::key{KEY_PPAGE}},
                    {0x3b,
                     {{
                         {0x32, {{{0x7e, util::key{KEY_PPAGE, true}}}}},
                         {0x35, {{{0x7e, util::key{KEY_PPAGE, false, true}}}}},
                     }}}}}},
                 {0x36,
                  {{{0x7e, util::key{KEY_NPAGE}},
                    {0x3b,
                     {{
                         {0x32, {{{0x7e, util::key{KEY_NPAGE, true}}}}},
                         {0x35, {{{0x7e, util::key{KEY_NPAGE, false, true}}}}},
                     }}}}}},
             }}},
            {0x4f,
             {{
                 {0x50, {KEY_F(1)}},
                 {0x51, {KEY_F(2)}},
                 {0x52, {KEY_F(3)}},
                 {0x53, {KEY_F(4)}},
             }}},
        }}}}};
  if (!codes.empty()) {
    auto offset = 0;
    auto k = keymap;
    for (auto i = 0; i < codes.size(); i++) {
      k = k[codes[i]];
    }
    if (k.code().decode != 0) {
      cmd = k.code();
      cmd.raw = codes;
      command(cmd);
      return;
    } else {
      for (auto &code : codes) {
        command({
            code,
            false,
            false,
            false,
            {code},
        });
        char str[1024];
        sprintf(str, "0x%x", code);
        mvprintw(10 + offset, 10, str);
        offset++;
      }
      refresh();
    }
  }
}