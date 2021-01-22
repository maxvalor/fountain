#ifndef SAMPLE_SRV_H_
#define SAMPLE_SRV_H_

#include <fountain/service.h>

struct SampleSrv : public ftn::Service
{
  std::uint32_t req;
  std::uint32_t resp;
};

#endif
