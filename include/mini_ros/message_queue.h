#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include "message.h"
#include "service.h"
#include "multithread_queue.h"

namespace mini_ros {

using MessagePair = std::pair<std::string, std::shared_ptr<Message>>;

using MessageQueue = MultiThreadQueue<MessagePair>;

using ServicePair = std::pair<std::string, std::shared_ptr<Service>>;

using ServiceQueue = MultiThreadQueue<ServicePair>;

}
#endif
