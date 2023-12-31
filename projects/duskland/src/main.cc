﻿#define FMT_HEADER_ONLY
#include "core/include/singleton.hpp"
#include "runtime/include/application.hpp"
#include <iostream>
#include <signal.h>
using namespace duskland;

int main(int argc, char *argv[]) {
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  auto app = core::singleton<runtime::application>::get();
  try {
    app->initialize(argc, argv);
    return app->run();
  } catch (std::exception &e) {
    app->exit();
    app->uninitialize();
    std::cerr << e.what() << std::endl;
    return -1;
  }
}