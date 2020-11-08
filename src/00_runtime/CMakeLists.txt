project(00_runtime CXX)

add_executable(00_runtime main.cc)

target_compile_features(00_runtime PUBLIC cxx_std_11)
