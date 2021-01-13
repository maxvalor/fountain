#include <mini_ros/mini_ros.h>
#include "sub_module.h"


int main()
{
  mini_ros::init();
  SubModule sm;
  mini_ros::hold(&sm);
  return 0;
}
