#ifndef THREAD_HANDLE_H_
#define THREAD_HANDLE_H_

#include <thread>
#include <assert.h>
#include <future>
#include <chrono>
#include <list>
#include <string.h>
#include "message_queue.h"
#include "core.h"
#include "subscriber.h"
#include "publisher.h"

namespace mini_ros {

class ThreadHandle {
public:
  ThreadHandle();
  virtual ~ThreadHandle ();

  void spin();
  void stop();

  template <typename T>
  Subscriber subscribe(std::string topic, std::function<void(std::shared_ptr<T>)> f)
  {

    auto packed_f = [f](std::shared_ptr<Message> msg)
    {
      // auto mem = new std::uint8_t[sizeof(T)];
      // auto pMsg = reinterpret_cast<T*>(mem);
      // auto n_msg = std::shared_ptr<T>(pMsg);
      // memcpy(pMsg, (void*)msg.get(), sizeof(T));
      // f(n_msg);

      f(std::static_pointer_cast<T>(msg));
    };

    return _subscribe(topic, packed_f);
  }

  template <typename T>
  Publisher advertise(std::string topic)
  {
    return _advertise(topic, typeid(T).name());
  }

private:
  struct function_pair {
    std::function<void(std::shared_ptr<Message>)> f;
    bool enable;
  };
  using func_vector = std::list<function_pair>;
  std::map<std::string, func_vector> topic_callbacks;
  std::mutex tcb_mtx;

  MessageQueue recv_msg_queue;
  MessageQueue send_msg_queue;
  MessageQueue send_msg_queue_mosquitto;
  bool running;
  std::thread::id tid;


  Subscriber _subscribe(std::string& topic,
    std::function<void(std::shared_ptr<Message>)> f);
  Publisher _advertise(std::string& topic, const char* tname);
};

} /* mini_ros */

#endif
