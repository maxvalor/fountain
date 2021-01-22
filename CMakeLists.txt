cmake_minimum_required(VERSION 3.0)

project(fountain VERSION 1.0)

add_definitions(-std=c++11)

include_directories(
  include/fountain
)

add_library(fountain SHARED
  src/core.cpp
  src/main_thread.cpp
  src/module.cpp
  src/thread_handle.cpp
  src/convenient.cpp
)

target_link_libraries(fountain
  pthread
  mosquitto
)

include(cmake/sample.cmake)

install(TARGETS fountain
  LIBRARY DESTINATION lib
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)
