cmake_minimum_required(VERSION 3.10)

project (bosch_queue)
set(CMAKE_CXX_STANDARD 17)
add_executable(bosch_queue src/bosch_queue.cpp)
# target_link_libraries(bosch_queue gdb)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/CppUnit/include
)

option (UNIT_TEST "Build unit tests" ON)
if (UNIT_TEST)
  enable_testing ()
  add_subdirectory ( "${CMAKE_SOURCE_DIR}/CppUnit" )
  add_subdirectory ( "${CMAKE_SOURCE_DIR}/test" )
else()
  message("Skip unit tests")
endif()

option (GENERATE_DOCS "Generate documentation" ON)
if (GENERATE_DOCS)
  add_custom_command(
    TARGET bosch_queue
    POST_BUILD
    COMMAND doxygen ${CMAKE_SOURCE_DIR}/bosch_queue.doxygen
    VERBATIM)
else()
  message("Skip document generation")
endif()