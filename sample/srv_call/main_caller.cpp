#include <mini_ros/mini_ros.h>
#include "client_module.h"


int main()
{
  mini_ros::init();
  ClientModule cm;
  mini_ros::hold(&cm);
  return 0;
}
