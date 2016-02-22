cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(final)	

find_package(Libev REQUIRED)
find_package(MAGIC REQUIRED)

include_directories(includes ${LIBEV_INCLUDE_DIR} ${MAGIC_INCLUDE_DIR})

add_executable(final main.cpp src/server.cpp src/daemon.cpp src/request.cpp src/filetype.cpp)

find_package(Threads REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
  set_property(TARGET final PROPERTY COMPILE_OPTIONS "-pthread")
  set_property(TARGET final PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(final ${CMAKE_THREAD_LIBS_INIT})
endif()

target_link_libraries(final ${LIBEV_LIBRARIES} ${MAGIC_LIBRARIES})