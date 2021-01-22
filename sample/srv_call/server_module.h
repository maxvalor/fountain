#ifndef SERVER_MODULE_H_
#define SERVER_MODULE_H_

#include <fountain/module.h>
#include <iostream>
#include "sample_srv.h"

class ServerModule : public ftn::Module {
private:
  /* data */

public:
  ServerModule () {}
  virtual ~ServerModule () {}

  bool onCalled(std::shared_ptr<SampleSrv> srv)
  {
    std::cout << "receive request:" << srv->req << std::endl;
    srv->resp = srv->req + 100;

    return true;
  }

  void onInit() override
  {
    ftn::ServiceServer server =
      getModuleHandle().advertiseService<SampleSrv>("sample_service",
        std::bind(&ServerModule::onCalled, this, std::placeholders::_1));
  }

  void onStopped() override
  {
    std::cout << "server exit." << std::endl;
  }
};

#endif
