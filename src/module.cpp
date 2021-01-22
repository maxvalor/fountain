#include "module.h"
#include <iostream>

namespace ftn {

  Module::Module()
  {
    stopped = false;

    _t = std::thread([this]()
    {
      handle = new ModuleHandle();
      onInit();
      handle->spin();
      onExit();
      while (!stopped)
      {
        cv.notify_all();
        std::this_thread::yield();
      }
    });
  }

  void Module::wait()
  {
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck);
    stopped = true;
  }

  void Module::sleep(uint32_t ms)
  {
    assert(std::this_thread::get_id() == _t.get_id());
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

  void Module::stop()
  {
    onStopped();
    handle->stop();
  }

  void Module::onExit() {}
  void Module::onStopped() {}

  Module::~Module()
  {
    if (handle != nullptr)
    {
      delete[] handle;
    }
  }

  ModuleHandle& Module::getModuleHandle()
  {
    return *handle;
  }

} /* ftn */
