cd "$(dirname "$0")"
cmake -S . -B .build -DTABLE_LIMIT=1000 -DCMAKE_TOOLCHAIN_FILE=toolchain/clang_debug.cmake
cmake --build .build --parallel 8
