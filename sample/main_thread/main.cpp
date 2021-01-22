#include <fountain/fountain.h>
#include "ros_module.h"


int main()
{
  ftn::init();
  ROSModule rm;
  ftn::hold(&rm);
  return 0;
}
