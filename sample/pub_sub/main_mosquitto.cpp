#include <fountain/fountain.h>
#include "sub_module.h"


int main()
{
  ftn::init();
  SubModule sm;
  ftn::hold(&sm);
  return 0;
}
