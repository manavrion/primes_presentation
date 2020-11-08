cmake_minimum_required(VERSION 3.8)

project(primes_presentation CXX)

set(TABLE_LIMIT
    0
    CACHE STRING "Limit of the prime table")
if(NOT TABLE_LIMIT MATCHES "[0-9]+$")
  message(FATAL_ERROR "TABLE_LIMIT must be a number")
endif()

add_definitions(-DTABLE_LIMIT=${TABLE_LIMIT})

add_subdirectory(benchmark)
add_subdirectory(src)
