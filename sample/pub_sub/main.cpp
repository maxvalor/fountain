#include <fountain/fountain.h>
#include "pub_module.h"
#include "sub_module.h"


int main()
{
  ftn::init();
  SubModule sm;
  PubModule pm;
  ftn::hold(&pm, &sm);
  return 0;
}
