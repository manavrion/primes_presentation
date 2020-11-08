cd "$(dirname "$0")"
rm -rf .benchbuild
mkdir .benchbuild
cd .benchbuild
../.build/benchmark/macrobench
