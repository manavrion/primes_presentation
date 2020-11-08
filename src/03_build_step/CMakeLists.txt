project(03_build_step CXX)

add_executable(03_build_step main.cc)

target_compile_features(03_build_step PUBLIC cxx_std_11)
