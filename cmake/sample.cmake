# sample.cmake

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
)

include_directories(
  include
  sample/pub_sub/
  sample/threadhandler/
  sample/main_thread/
  ${catkin_INCLUDE_DIRS}
)

add_executable(sample_ps sample/pub_sub/main.cpp)
add_executable(sample_ps_s sample/pub_sub/main_mosquitto.cpp)
add_executable(sample_th sample/threadhandler/main.cpp)
add_executable(sample_mt sample/main_thread/main.cpp)

set(SAMPLE_DEPENDS miniros)
target_link_libraries(sample_ps ${SAMPLE_DEPENDS})
target_link_libraries(sample_ps_s ${SAMPLE_DEPENDS})
target_link_libraries(sample_th ${SAMPLE_DEPENDS})
target_link_libraries(sample_mt
  ${SAMPLE_DEPENDS}
  ${catkin_LIBRARIES}
)
