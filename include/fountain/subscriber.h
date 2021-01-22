#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <functional>

namespace ftn {

class Subscriber {
  std::function<void(void)> f;

  friend class ThreadHandle;
  Subscriber(std::function<void(void)> f) : f(f) {}
public:
  Subscriber() : f(nullptr) {}
  void shutdown() {
    if (f != nullptr ) {
      f();
    }
  }
};

} /* ftn */

#endif
