#ifndef SAMPLE_MSG_H_
#define SAMPLE_MSG_H_

#include <fountain/message.h>
#include "sample_msg.h"

class ImageTestType
{
public:
  std::uint8_t data[1920 * 1080 * 4 / 20]; // a 1080P png image
};  

struct SampleMsg : public ftn::Message {
  // std::uint32_t data;
  struct timeval tv_start;
  ImageTestType image;
};
#endif
