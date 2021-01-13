#include <mini_ros/mini_ros.h>
#include "pub_module.h"
#include "sub_module.h"


int main()
{
  mini_ros::init();
  SubModule sm;
  PubModule pm;
  mini_ros::hold(&pm, &sm);
  return 0;
}
