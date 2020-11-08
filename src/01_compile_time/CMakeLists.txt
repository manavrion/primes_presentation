cmake_minimum_required(VERSION 3.8)
project(01_get_primes)

set(SOURCE_FILES main.cc)

source_group("sources" FILES ${SOURCE_FILES})

add_executable(01_get_primes ${SOURCE_FILES})

target_compile_features(01_get_primes PUBLIC cxx_std_17)
