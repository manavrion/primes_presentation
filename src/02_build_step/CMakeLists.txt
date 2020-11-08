project(02_build_step CXX)

add_executable(02_build_step main.cc)

target_compile_features(02_build_step PUBLIC cxx_std_11)
