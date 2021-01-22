#ifndef ROS_MODULE_H_
#define ROS_MODULE_H_

#include <fountain/module.h>
#include <fountain/main_thread.h>
#include <iostream>
#include <ros/ros.h>
#include <std_msgs/String.h>

class ROSModule : public ftn::Module {
private:
  ros::NodeHandle *nh;

public:
  ROSModule () {}
  virtual ~ROSModule () {}

  void onMsg();

  void onInit() override
  {
    ros::Publisher pub;
    ftn::MainThreadEvent init_event([this]()
    {
      int argc = ftn::argc()();
      char** argv = ftn::argv()();
      ros::init(argc, argv, "test_node", ros::init_options::NoSigintHandler);
      nh = new ros::NodeHandle();
    });
    ftn::main_thread_wait(init_event);

    std::cout << "success to start ros." << std::endl;
    std::thread([this]()
    {
      bool ros_ok = true;
      ros::Rate loop_rate(40);
      ftn::MainThreadEvent ok_event([&ros_ok]()
      {
        ros_ok = ros::ok();
      });
      ftn::MainThreadEvent spin_event([&loop_rate]()
      {
        ros::spinOnce();
        loop_rate.sleep();
      });

      while (ros_ok)
      {
        ftn::main_thread_wait(spin_event);
        ftn::main_thread_wait(ok_event);
        std::cout << "ros ok:" << ros_ok << std::endl;
      }
    }).detach();
  }

  void onStopped() override
  {
    ros::shutdown();
    std::cout << "success to stop ros." << std::endl;
  }
};

#endif
