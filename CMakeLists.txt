cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Xenomai 3 REQUIRED)

add_executable(rt_main
  src/rt_main.cpp
)

target_include_directories(rt_main
  PUBLIC
  ${XENOMAI_INCLUDE_DIRS}
)

target_link_libraries(rt_main
  ${XENOMAI_LIBRARIES}
)
