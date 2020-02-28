cmake_minimum_required(VERSION 3.8)
project(03_get_primes_ranges)

set(SOURCE_FILES main.cc)

source_group("sources" FILES ${SOURCE_FILES})

add_executable(03_get_primes_ranges ${SOURCE_FILES})
