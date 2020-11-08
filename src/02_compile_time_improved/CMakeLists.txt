project(02_compile_time_improved CXX)

add_executable(02_compile_time_improved main.cc)

target_compile_features(02_compile_time_improved PUBLIC cxx_std_20)
