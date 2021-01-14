#include <mini_ros/mini_ros.h>
#include "server_module.h"


int main()
{
  mini_ros::init();
  ServerModule sm;
  mini_ros::hold(&sm);
  return 0;
}
