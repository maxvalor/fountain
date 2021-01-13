#ifndef MSG_H_
#define MSG_H_

#include <memory>

namespace mini_ros {
  struct Message {
    struct Header {
      std::uint32_t id;
      std::uint32_t timestamp;
      std::uint32_t data_len;
    } header;
    
  //   virtual ~Message() {}

  // static void operator delete(void* mem) 
  // { 
  //   if (mem != nullptr) 
  //   { 
  //     free(mem); 
  //   } 
  // } 
  };
}
#endif
