#include "core/singleton.hpp"
#include "system/application.hpp"
#include <iostream>
#include <ncurses.h>
using namespace duskland;
int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  auto app = core::singleton<system::application>::get();
  app->initialize(argc, argv);
  return app->run();
}
