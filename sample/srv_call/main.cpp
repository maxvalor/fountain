#include <fountain/fountain.h>
#include "server_module.h"
#include "client_module.h"


int main()
{
  ftn::init();
  ServerModule sm;
  ClientModule cm;
  ftn::hold(&sm, &cm);
  return 0;
}
