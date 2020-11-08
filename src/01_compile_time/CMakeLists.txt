project(01_compile_time CXX)

add_executable(01_compile_time main.cc)

target_compile_features(01_compile_time PUBLIC cxx_std_20)
