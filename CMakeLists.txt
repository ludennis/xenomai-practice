cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Xenomai 3 REQUIRED)

add_executable(message_queue
  src/message_queue_main.cpp
)

target_include_directories(message_queue
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
)

target_link_libraries(message_queue
  ${XENOMAI_LIBRARIES}
)

target_compile_options(message_queue
  PUBLIC
  -g
  -fpermissive
)
