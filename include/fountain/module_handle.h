#ifndef MODULE_HANDLE_H_
#define MODULE_HANDLE_H_

#include "thread_handle.h"

namespace ftn {

  class ModuleHandle : public ThreadHandle
  {
  private:
    friend class Module;
    ModuleHandle() {}
  };

} /* ftn */

#endif
