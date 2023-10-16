#include "core/singleton.hpp"
#include "system/application.hpp"
#include <signal.h>
using namespace duskland;
int main(int argc, char *argv[]) {
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);
  auto app = core::singleton<system::application>::get();
  app->initialize(argc, argv);
  return app->run();
}