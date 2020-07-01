cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Xenomai 3 REQUIRED)

# message queue send
add_executable(message_queue_send
  src/intertask_communication/message_queue_send.cpp
)

target_include_directories(message_queue_send
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
)

target_link_libraries(message_queue_send
  ${XENOMAI_LIBRARIES}
)

target_compile_options(message_queue_send
  PUBLIC
  -g
  -fpermissive
)

# message queue receive
add_executable(message_queue_receive
  src/intertask_communication/message_queue_receive.cpp
)

target_include_directories(message_queue_receive
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
)

target_link_libraries(message_queue_receive
  ${XENOMAI_LIBRARIES}
)

target_compile_options(message_queue_receive
  PUBLIC
  -g
  -fpermissive
)
