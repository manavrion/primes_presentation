# Tools
set(CMAKE_C_COMPILER clang-12)

set(CMAKE_CXX_COMPILER clang++-12)

set(CMAKE_AR llvm-ar-12)
set(CMAKE_LINKER llvm-link-12)
set(CMAKE_NM llvm-nm-12)
set(CMAKE_OBJDUMP llvm-objdump-12)
set(CMAKE_RANLIB llvm-ranlib-12)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fconstexpr-depth=1000 -fconstexpr-steps=1000000000")

# Build
set(CMAKE_BUILD_TYPE Release)
