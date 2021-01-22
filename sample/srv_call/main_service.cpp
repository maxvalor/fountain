#include <fountain/fountain.h>
#include "server_module.h"


int main()
{
  ftn::init();
  ServerModule sm;
  ftn::hold(&sm);
  return 0;
}
