#include <fountain/fountain.h>
#include "client_module.h"


int main()
{
  ftn::init();
  ClientModule cm;
  ftn::hold(&cm);
  return 0;
}
