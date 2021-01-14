#ifndef SERVICE_H_
#define SERVICE_H_

namespace mini_ros {

  struct Service {
    struct Header {
      pid_t pid;
      std::thread::id tid;
      std::uint32_t timestamp;
      size_t data_len;
    } header;

    std::uint8_t __data[0];
  };
} /* mini_ros */


#endif
