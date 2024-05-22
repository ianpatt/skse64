![testbuild](https://github.com/ianpatt/skse64/workflows/testbuild/badge.svg)
## Building
```
git clone https://github.com/ianpatt/common
git clone https://github.com/ianpatt/skse64
cmake -B common/build -S common -DCMAKE_INSTALL_PREFIX=extern common
cmake --build common/build --config Release --target install
cmake -B skse64/build -S skse64 -DCMAKE_INSTALL_PREFIX=extern skse64
cmake --build skse64/build --config Release
```
Solution will be generated at skse64/build/umbrella.sln.
