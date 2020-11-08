project(02_compile_time_bits CXX)

add_executable(02_compile_time_bits main.cc)

target_compile_features(02_compile_time_bits PUBLIC cxx_std_20)
