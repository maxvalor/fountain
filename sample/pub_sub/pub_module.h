#ifndef PUB_MODULE_H_
#define PUB_MODULE_H_

#include <fountain/module.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include "sample_msg.h"

class PubModule : public ftn::Module {
private:
  bool running;

public:
  PubModule () {}
  virtual ~PubModule () {}

  void onInit() override
  {
    ftn::Publisher pub = getModuleHandle().advertise<SampleMsg>("sample_topic");
    int i = 1000000;
    running = true;
    ftn::Rate loop(1000);
    while (running && --i)
    {
      SampleMsg msg;
      // msg.data = i;
      gettimeofday(&msg.tv_start, NULL);
      printf("pub start time: %lu s %lu us\n", msg.tv_start.tv_sec, msg.tv_start.tv_usec);
      // std::cout << "publish by object, data:" << msg.data << std::endl;
      std::cout << std::endl;
      pub.publish(msg);

      // std::shared_ptr<SampleMsg> spMsg(new SampleMsg());
      // spMsg->data = new std::uint32_t[1];
      // spMsg->data[0] = i;
      // spMsg->len = 1;
      // std::cout << "publish by shared pointer, data:" << spMsg->data[0] << std::endl;
      // pub.publish(spMsg);

      loop.sleep();
    }
  }

  void onStopped() override
  {
    running = false;
    // exit(0);
  }

  void onExit() override
  {
    std::cout << "publisher exit." << std::endl;
  }
};

#endif
