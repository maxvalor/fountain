#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include <functional>
#include "message.h"

namespace ftn {

class Publisher {
  using pub_func = std::function<void(std::shared_ptr<Message>, const char*)>;
  pub_func f;

  friend class ThreadHandle;
  Publisher(pub_func f) : f(f) {}

public:
  Publisher() : f(nullptr) {}
  template <typename T>
  void publish(T& msg, size_t data_len = sizeof(T)) {
    if (f != nullptr) {
      std::shared_ptr<Message> sMsg(new T(std::move(msg)));
      sMsg->header.data_len = data_len;
      f(sMsg, typeid(T).name());
    }
  }

  template <typename T>
  void publish(std::shared_ptr<T> msg, size_t data_len = sizeof(T)) {
    if (f != nullptr) {
      msg->header.data_len = data_len;
      f(msg, typeid(T).name());
    }
  }
};

} /* ftn */

#endif
