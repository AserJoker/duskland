#include "core/singleton.hpp"
#include "system/application.hpp"
#include <codecvt>
#include <iostream>
#include <locale>
#include <ncurses.h>
#include <signal.h>
using namespace duskland;
int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  auto app = core::singleton<system::application>::get();
  app->initialize(argc, argv);
  return app->run();
}
