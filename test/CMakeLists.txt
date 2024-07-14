cmake_minimum_required(VERSION 3.10)

project(test_queue_cpp_unit CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(test_queue test_queue.cpp ../include/bosch_queue.hpp)

target_link_libraries(test_queue cppunit)