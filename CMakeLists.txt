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
  src/common
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
  src/common
)

target_link_libraries(message_queue_receive
  ${XENOMAI_LIBRARIES}
)

target_compile_options(message_queue_receive
  PUBLIC
  -g
  -fpermissive
)

# task send
add_executable(task_send
  src/intertask_communication/task_send.cpp
)

target_include_directories(task_send
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(task_send
  ${XENOMAI_LIBRARIES}
)

# task receive
add_executable(task_receive
  src/intertask_communication/task_receive.cpp
)

target_include_directories(task_receive
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(task_receive
  ${XENOMAI_LIBRARIES}
)

# message pipe send
add_executable(message_pipe_send
  src/intertask_communication/message_pipe_send.cpp
)

target_include_directories(message_pipe_send
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(message_pipe_send
  ${XENOMAI_LIBRARIES}
)

# message pipe receive
add_executable(message_pipe_receive
  src/intertask_communication/message_pipe_receive.cpp
)

target_include_directories(message_pipe_receive
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(message_pipe_receive
  ${XENOMAI_LIBRARIES}
)

# round trip send
add_executable(round_trip_send
  src/timing/round_trip_send.cpp
)

target_include_directories(round_trip_send
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(round_trip_send
  ${XENOMAI_LIBRARIES}
)


# round trip receive
add_executable(round_trip_receive
  src/timing/round_trip_receive.cpp
)

target_include_directories(round_trip_receive
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
  src/common
)

target_link_libraries(round_trip_receive
  ${XENOMAI_LIBRARIES}
)
