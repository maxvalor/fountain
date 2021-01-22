#ifndef SERVICE_H_
#define SERVICE_H_

#include <sys/time.h>

namespace ftn {

  struct Service {
    struct Header {
      pid_t pid;
      std::thread::id tid;
      struct timeval timestamp;
      size_t data_len;
    } header;

    std::uint8_t __data[0];
  };
} /* ftn */


#endif
