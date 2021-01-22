#include "core.h"

#include <string.h>
#include <iostream>

namespace ftn {

  Core* Core::singleton = nullptr;

  Core& Core::instance() {
    static std::mutex mtx;
    if (singleton == nullptr) {
      mtx.lock();
      if (singleton == nullptr) {
        singleton = new Core();
      }
      mtx.unlock();
    }

    return *singleton;
  }

  // std::shared_ptr<Message> test_s[100];

  void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
  {
    /* This blindly prints the payload, but the payload can be anything so take care. */
    // printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
    if (msg->payloadlen > 1)
    {
      // struct timeval tv_start, tv_end;
      // gettimeofday(&tv_end, NULL);
      // printf("received.\n");
      // memcpy(&tv_start, msg->payload, sizeof(tv_start));
      // printf("sub start time: %lu s %lu us\n", tv_start.tv_sec, tv_start.tv_usec);
      // printf("sub end   time: %lu s %lu us\n", tv_end.tv_sec, tv_end.tv_usec);
      // printf("sub delay     : %lf ms\n", ((tv_end.tv_sec - tv_start.tv_sec) * 1000 + (tv_end.tv_usec - tv_start.tv_usec) / 1000.0));
      std::cout << "on_message payload:" << msg->payloadlen << std::endl;
      auto len = msg->payloadlen;
      if (len > sizeof(Message))
      {
        auto mem = new std::uint8_t[len];
        auto pMsg = reinterpret_cast<Message*>(mem);
        memcpy(mem, msg->payload, len);
        MessagePair msg_pair(
          std::string(msg->topic),
          std::shared_ptr<Message>(pMsg)
        );
        Core::instance().deliver(msg_pair);
      }
    }
    else
    {
      printf("error frame.\n");
    }

    // printf("delay:%lfms\n", ((tv_end.tv_usec - tv_start.tv_usec) / 1000.0));
  }

  void on_srv_req(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
  {
    /* This blindly prints the payload, but the payload can be anything so take care. */
    // printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
    if (msg->payloadlen > 1)
    {
      std::cout << "on_srv_call payload:" << msg->payloadlen << std::endl;
      auto len = msg->payloadlen;
      if (len > sizeof(Message))
      {
        auto mem = new std::uint8_t[len];
        auto pMsg = reinterpret_cast<Service*>(mem);
        memcpy(mem, msg->payload, len);
        Core::instance().call_service(std::string(msg->topic), std::shared_ptr<Service>(pMsg), true);
      }
    }
    else
    {
      printf("error frame.\n");
    }

    // printf("delay:%lfms\n", ((tv_end.tv_usec - tv_start.tv_usec) / 1000.0));
  }

  void on_srv_resp(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
  {
    /* This blindly prints the payload, but the payload can be anything so take care. */
    // printf("%s %d %s\n", msg->topic, msg->qos, (char *)msg->payload);
    if (msg->payloadlen > 1)
    {
      std::cout << "on_srv_call payload:" << msg->payloadlen << std::endl;
      auto len = msg->payloadlen;
      if (len > sizeof(Message))
      {
        auto mem = new std::uint8_t[len];
        auto pMsg = reinterpret_cast<Service*>(mem);
        memcpy(mem, msg->payload, len);
        ServicePair msg_pair(
          std::string(msg->topic),
          std::shared_ptr<Service>(pMsg)
        );
        Core::instance().deliver(msg_pair);
      }
    }
    else
    {
      printf("error frame.\n");
    }

    // printf("delay:%lfms\n", ((tv_end.tv_usec - tv_start.tv_usec) / 1000.0));
  }


  Core::Core()
  {
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
	  mosquitto_int_option(mosq, MOSQ_OPT_TCP_NODELAY, 1);
	  mosquitto_int_option(mosq, MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V5);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_connect(mosq, "127.0.0.1", 1883, 60);
    mosquitto_loop_start(mosq);

    mosq_req = mosquitto_new(NULL, true, NULL);
	  mosquitto_int_option(mosq_req, MOSQ_OPT_TCP_NODELAY, 1);
	  mosquitto_int_option(mosq_req, MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V5);
    mosquitto_message_callback_set(mosq_req, on_srv_req);
    mosquitto_connect(mosq_req, "127.0.0.1", 1883, 60);
    mosquitto_loop_start(mosq_req);

    mosq_resp = mosquitto_new(NULL, true, NULL);
	  mosquitto_int_option(mosq_resp, MOSQ_OPT_TCP_NODELAY, 1);
	  mosquitto_int_option(mosq_resp, MOSQ_OPT_PROTOCOL_VERSION, MQTT_PROTOCOL_V5);
    mosquitto_message_callback_set(mosq_resp, on_srv_resp);
    mosquitto_connect(mosq_resp, "127.0.0.1", 1883, 60);
    mosquitto_loop_start(mosq_resp);
  }

  Core::~Core()
  {
    mosquitto_disconnect(mosq);
    mosquitto_disconnect(mosq_req);
    mosquitto_disconnect(mosq_resp);
    mosquitto_lib_cleanup();
  }

  void Core::register_handle(std::thread::id tid,
    std::function<void(MessagePair)> f)
  {
    std::lock_guard<std::mutex> lck(emplaces_mtx);
    emplaces.insert(std::pair<std::thread::id,
      std::function<void(MessagePair)>>(tid, f));
  }

  void Core::subscribe(std::thread::id tid, std::string topic)
  {
    std::lock_guard<std::mutex> lck(subscribers_mtx);
    try {
      auto& thread_ids = subscribers.at(topic);
      for (auto id : thread_ids)
      {
        if (id == tid)
        {
          return;
        }
      }
      thread_ids.emplace_back(tid);
    }
    catch (std::out_of_range e) {
      std::list<std::thread::id> thread_ids;
      thread_ids.emplace_back(tid);
      subscribers.insert(
        std::pair<std::string, std::list<std::thread::id>>(topic, thread_ids));
      mosquitto_subscribe_v5(mosq, NULL, topic.c_str(), 2, MQTT_SUB_OPT_NO_LOCAL | MQTT_SUB_OPT_SEND_RETAIN_NEVER, NULL);
    }
  }

  void Core::deliver(ServicePair msg)
  {
    srv_resp_queue.emplace(msg);
    srv_resp_queue.notify();
  }

  void Core::deliver(MessagePair msg, bool mosquitto)
  {
    if (mosquitto) 
    {
      mosquitto_publish(mosq, NULL, msg.first.c_str(), msg.second->header.data_len, msg.second.get(), 2, false);
      std::cout << "send data len:" << msg.second->header.data_len << std::endl;
      std::cout << std::endl;
    }
    else
    {
      std::lock_guard<std::mutex> lck(subscribers_mtx);
      std::lock_guard<std::mutex> lck2(emplaces_mtx);
      try {
        auto& tids = subscribers.at(msg.first);
        for (auto tid : tids)
        {
          try {
            auto emplace = emplaces.at(tid);
            emplace(msg);
          }
          catch (std::out_of_range e) {
            // do nothing
          }
        }
      }
      catch (std::out_of_range e) {
      }
    }
  }

  void Core::register_service(std::string srv_name,
    std::function<bool(std::shared_ptr<Service>)> f)
  {
    std::lock_guard<std::mutex> lck(services_mtx);
    try
    {
      auto& f_n = services.at(srv_name);
      f_n = f;
    }
    catch (std::out_of_range e)
    {
      services.insert(std::pair<std::string,
          std::function<bool(std::shared_ptr<Service>)>>(srv_name, f));
    }

    std::string srv_call = srv_name;
    mosquitto_subscribe_v5(mosq_req, NULL, srv_call.c_str(), 2, MQTT_SUB_OPT_NO_LOCAL | MQTT_SUB_OPT_SEND_RETAIN_NEVER, NULL);
  }

  bool Core::wait_for_response(std::string srv_resp, std::shared_ptr<Service> srv)
  {
    // int i;
    // for (i = 0; i < 5; ++i) {
    //   if (!srv_resp_queue.empty()) {
    //     auto pair = srv_resp_queue.front();
    //     if (
    //       srv->header.pid == pair.second->header.pid 
    //       && srv->header.tid == pair.second->header.tid
    //       // && srv->header.timestamp == pair.second->header.timestamp
    //     )
    //     {
    //       memcpy(srv.get(), pair.second.get(), srv->header.data_len);
    //       srv_resp_queue.pop();
    //     }
    //   }
    //   else {
    //     srv_resp_queue.wait();
    //   }
    // }

    // return i < 5;

    while (1) {
      if (!srv_resp_queue.empty()) {
        auto pair = srv_resp_queue.front();
        if (
          srv->header.pid == pair.second->header.pid 
          && srv->header.tid == pair.second->header.tid
          && srv_resp.compare(pair.first) == 0
          // && srv->header.timestamp == pair.second->header.timestamp
        )
        {
          memcpy(srv.get(), pair.second.get(), srv->header.data_len);
          srv_resp_queue.pop();
          break;
        }
      }
      else {
        srv_resp_queue.wait();
      }
    }

    return true;
  }

  bool Core::call_service(std::string srv_name,
    std::shared_ptr<Service> srv, bool remote)
  {
    bool found = false;
    if (remote)
    {
      services_mtx.lock();
      auto f = services.at(srv_name);
      services_mtx.unlock();
      f(srv);
      std::string srv_resp = std::string("~") + srv_name;
      mosquitto_publish(mosq_resp, NULL, srv_resp.c_str(), srv->header.data_len, srv.get(), 2, false);
      return true;
    }
    else
    {
      try
      {
        services_mtx.lock();
        auto f = services.at(srv_name);
        services_mtx.unlock();
        return f(srv);
      }
      catch (std::out_of_range e)
      {
        std::string srv_call = srv_name;
        std::string srv_resp = std::string("~") + srv_name;
        std::cout << srv_resp.substr(1) << std::endl;
        mosquitto_subscribe_v5(mosq_resp, NULL, srv_resp.c_str(), 2, MQTT_SUB_OPT_NO_LOCAL | MQTT_SUB_OPT_SEND_RETAIN_NEVER, NULL);
        mosquitto_publish(mosq_req, NULL, srv_call.c_str(), srv->header.data_len, srv.get(), 2, false);
        services_mtx.unlock();

        found = wait_for_response(srv_resp, srv);
      }
    }
    return found;
  }
} /* ftn */
