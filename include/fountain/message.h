#ifndef MSG_H_
#define MSG_H_

#include <memory>

namespace ftn {
  struct Message {
    struct Header {
      std::uint32_t id;
      std::uint32_t timestamp;
      size_t data_len;
    } header;

    std::uint8_t __data[0];
  };
}
#endif
