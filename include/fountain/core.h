#ifndef CORE_H_
#define CORE_H_

#include <map>
#include <list>
#include <thread>
#include <functional>
#include "message_queue.h"
#include "service.h"
#include <mosquitto.h>
#include <mqtt_protocol.h>

namespace ftn {

class Core {
private:
  std::map<std::thread::id,
    std::function<void(MessagePair)>> emplaces;

  std::map<std::string, std::list<std::thread::id>> subscribers;
  std::map<std::string, std::function<bool(std::shared_ptr<Service>)>> services;

  static Core *singleton;
  std::mutex emplaces_mtx;
  std::mutex subscribers_mtx;
  std::mutex services_mtx;
  struct mosquitto *mosq;
  struct mosquitto *mosq_req;
  struct mosquitto *mosq_resp;

  ServiceQueue srv_resp_queue;
  std::mutex srv_resp_mtx;

  Core();
public:

  static Core& instance();
  virtual ~Core ();

  void register_handle(std::thread::id tid,
    std::function<void(MessagePair)> f);

  void subscribe(std::thread::id tid, std::string topic);

  void deliver(MessagePair msg, bool mosquitto = false);
  void deliver(ServicePair msg);

  void register_service(std::string srv_name,
    std::function<bool(std::shared_ptr<Service>)> f);

  bool call_service(std::string srv_name, std::shared_ptr<Service> srv, bool remote = false);
  bool wait_for_response(std::string srv_resp, std::shared_ptr<Service> srv);
};

} /* ftn */
#endif
