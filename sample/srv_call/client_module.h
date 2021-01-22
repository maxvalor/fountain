#ifndef CLIENT_MODULE_H_
#define CLIENT_MODULE_H_

#include <fountain/module.h>
#include <iostream>
#include "sample_srv.h"

class ClientModule : public ftn::Module {
private:
  /* data */

public:
  ClientModule () {}
  virtual ~ClientModule () {}

  void onInit() override
  {
    ftn::ServiceClient client =
      getModuleHandle().serviceClient<SampleSrv>("sample_service");

    int i = 100;
    ftn::Rate loop(1);
    while (--i)
    {
      auto pSrv = std::make_shared<SampleSrv>();
      pSrv->req= i;
      if (client.call(pSrv))
      {
        std::cout << "call service by shared pointer, service return:" << pSrv->resp<< std::endl;
      }
      else
      {
        std::cout << "error " << std::endl;
      }

      loop.sleep();
    }
  }
  
  void onStopped() override
  {
    exit(0);
  }
};

#endif
