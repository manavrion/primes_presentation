cd "$(dirname "$0")"
cmake -S . -B .build -DTABLE_LIMIT=1000 -DCMAKE_TOOLCHAIN_FILE=toolchain/clang_release.cmake
cmake --build .build --parallel 8
