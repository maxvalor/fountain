#ifndef SERVICE_SERVER_H_
#define SERVICE_SERVER_H_

#include <functional>

namespace ftn {

class ServiceServer {
private:
  std::function<void(void)> f;

  friend class ThreadHandle;
  ServiceServer(std::function<void(void)> f) : f(f) {}
public:

  ServiceServer() : f(nullptr) {}
  virtual ~ServiceServer () {}
  void shutdown() {
    if (f != nullptr ) {
      f();
    }
  }
};

} /* ftn */

#endif
