#ifndef SUB_MODULE_H_
#define SUB_MODULE_H_

#include <fountain/module.h>
#include <iostream>
#include <sys/time.h>
#include "sample_msg.h"

class SubModule : public ftn::Module {
private:
  ftn::Subscriber sub;

public:
  SubModule () {}
  virtual ~SubModule () {}

  void onMsg(std::shared_ptr<SampleMsg> msg)
  {
    // if (msg->data[0] == 9999990)
    // {
    //   std::cout << "stop." << std::endl;
    //   sub.shutdown();
    // }
    // std::cout << "receive data:" << msg->data << std::endl;
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_end, NULL);
		printf("received.\n");
    
    tv_start = msg->tv_start;
		printf("sub start time: %lu s %lu us\n", tv_start.tv_sec, tv_start.tv_usec);
		printf("sub end   time: %lu s %lu us\n", tv_end.tv_sec, tv_end.tv_usec);
		printf("sub delay     : %lf ms\n", ((tv_end.tv_sec - tv_start.tv_sec) * 1000 + (tv_end.tv_usec - tv_start.tv_usec) / 1000.0));
    std::cout << std::endl;
  }

  void onInit() override
  {
    sub =
      getModuleHandle().subscribe<SampleMsg>("sample_topic",
        std::bind(&SubModule::onMsg, this, std::placeholders::_1));

    // ftn::Subscriber sub2 =
    //   getModuleHandle().subscribe<SampleMsg>("sample_topic",
    //     std::bind(&SubModule::onMsg, this, std::placeholders::_1));
  }

  void onExit() override
  {
    std::cout << "subscriber exit." << std::endl;
    exit(0);
  }
};

#endif
