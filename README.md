# Releases

## Nightly builds 

URL of the build from the HEAD commit of the Main branch:

* https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js

URLs of all builds on Main branch of the past 30 days:

* https://vizzu-lib-main-sha.storage.googleapis.com/lib-HASH/vizzu.js

where HASH is the first 7 character of the commit's git hash.

# License

Copyright © 2021 [Vizzu Kft.](https://vizzuhq.com).

Released under the [Apache 2.0 License](LICENSE).

# Setting up and building Vizzu on Ubuntu 20.04

## Install all build dependencies

### Install WASM version build dependencies

#### Method 1. Using PPA (recommended)

Add Vizzu PPA to the repository list:

```
wget -O - https://vizzuhq.github.io/ppa/ubuntu/KEY.gpg | sudo apt-key add -
sudo add-apt-repository "deb https://vizzuhq.github.io/ppa/ubuntu ./"
```

Install build dependencies:

```
sudo apt-get install vizzu-devenv
```

#### Method 2. Manual install

```
sudo apt-get install git, cmake, qt5-default, zlib1g-dev
cd $HOME
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 1.40.1
./emsdk activate 1.40.1
echo 'source "$HOME/emsdk/emsdk_env.sh"' >> $HOME/.bashrc
```

### Install Desktop version build dependencies

Add LLVM to the repository list:

```
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-12 main"
```

Install build dependencies:

```
sudo apt-get update
sudo apt-get install clang-12 clang-tools-12 lldb-12 lld-12 clang-tidy-12 clang-format-12 g++-10
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-12 120
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-12 120
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-12 120
```

## Building the project

### Getting the source code

```
cd $HOME
git clone git@github.com:vizzuhq/vizzu-lib.git
```

### Building

#### Building Desktop version

```
cd $HOME/vizzu-lib
mkdir -p build/cmake-desktop
cd build/cmake-desktop
cmake ../../project/cmake/
make
```

Note: build will fail if build directory placed outside of the repository.

Build with additional static analizer checks:

```
cmake ../../project/cmake/ -Dclangtidy=ON -Dcppcheck=ON
make
```

#### Building WASM version

NOTE: new terminal session needed after install, because emsdk environment setup runs on session begin from bashrc.

```
cd $HOME/vizzu-lib
mkdir -p build/cmake-wasm
cd build/cmake-wasm
emcmake cmake ../../project/cmake/
make
```

#### Unit testing

Run unit tests:

```
make test
```

Read detailed output log:

```
cat Testing/Temporary/LastTest.log
```

Run unit tests (with detailed output log):

```
ctest -V
```

### Debugging

#### Debug WASM version under Chrome

- set Chrome/DevTools/Settings/Experiments/'WebAssembly Debugging: Enable DWARF support' to true
- set [repo]/project/cmake/emcc.txt: CMAKE_EXE_LINKER_FLAGS_DEBUG --source-map-base to the URL where the browser can find cvizzu.wasm.map file

### Testing

[Integration testing](test/integration/README.md)
