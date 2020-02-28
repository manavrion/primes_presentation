cmake_minimum_required(VERSION 3.8)
project(00_runtime)

set(SOURCE_FILES main.cc)

source_group("sources" FILES ${SOURCE_FILES})

add_executable(00_runtime ${SOURCE_FILES})
