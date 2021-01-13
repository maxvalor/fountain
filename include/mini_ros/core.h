#ifndef CORE_H_
#define CORE_H_

#include <map>
#include <list>
#include <thread>
#include <functional>
#include "message_queue.h"
#include <mosquitto.h>
#include <mqtt_protocol.h>

namespace mini_ros {

class Core {
private:
  std::map<std::thread::id,
    std::function<void(MessagePair)>> emplaces;

  std::map<std::string, std::list<std::thread::id>> subscribers;

  static Core *singleton;
  std::mutex emplaces_mtx;
  std::mutex subscribers_mtx;
  struct mosquitto *mosq;

  Core();
public:

  static Core& instance();
  virtual ~Core ();

  void register_handle(std::thread::id tid,
    std::function<void(MessagePair)> f);

  void subscribe(std::thread::id tid, std::string topic);

  void deliver(MessagePair msg, bool mosquitto = false);
};

} /* mini_ros */
#endif
