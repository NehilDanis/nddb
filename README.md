# nddb
The work in this repository done following "Build a Debugger" from Sy Brand

# How to Use it?

This project uses vcpkg as package manager. Clone the [vcpkg repository](https://github.com/microsoft/vcpkg) outside of this project folder. 

Then run the bootstrap script as following
```shell
./vcpkg/bootstrap-vcpkg.sh
```
To prevent collection of usage data pass the following flag `-disableMetrics` to above command.

## Build

```shell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build -j4
```

## Run
To run the command line too use the following command:

```shell
./build/tools/nddb
```