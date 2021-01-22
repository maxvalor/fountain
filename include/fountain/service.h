#ifndef SERVICE_H_
#define SERVICE_H_

namespace ftn {

  struct Service {
    struct Header {
      pid_t pid;
      std::thread::id tid;
      std::uint32_t timestamp;
      size_t data_len;
    } header;

    std::uint8_t __data[0];
  };
} /* ftn */


#endif
