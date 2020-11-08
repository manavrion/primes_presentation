project(macrobench CXX)

add_executable(macrobench macrobench.cc)

target_compile_features(macrobench PUBLIC cxx_std_20)
