# Setting up and building Vizzu on Ubuntu 22.04

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

Install nodejs:

```
wget --quiet -O - https://deb.nodesource.com/setup_18.x | sudo bash
sudo apt-get update
sudo apt-get install nodejs
```

#### Method 2. Manual install

```
sudo apt-get install git cmake qtbase5-dev
cd $HOME
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 3.1.41
./emsdk activate 3.1.41
echo 'source "$HOME/emsdk/emsdk_env.sh"' >> $HOME/.bashrc
```

### Install Desktop version build dependencies

Add LLVM to the repository list:

```
sudo add-apt-repository ppa:deadsnakes/ppa -y
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
sudo add-apt-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-16 main"
```

Install build dependencies:

```
sudo apt-get update
sudo apt-get install g++-12 clang-16 clang-tools-16 lldb-16 lld-16 clang-tidy-16 clang-format-16 cppcheck
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-16 120
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-16 120
sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-16 120
sudo ln -s /usr/bin/clang-tidy-16 /usr/bin/clang-tidy
sudo ln -s /usr/bin/clang-format-16 /usr/bin/clang-format
```

## Building the project

### Getting the source code

```
git clone git@github.com:vizzuhq/vizzu-lib.git
```

### Building

#### Building Desktop version

```
cd vizzu-lib
mkdir -p build/cmake-desktop
cd build/cmake-desktop
cmake ../../project/cmake/
cmake --build .
```

Note: build will fail if build directory placed outside of the repository.

Note: A successful gcc and a clang build are required to contribute, just like
successful format checks and linter checks (on the `cvizzu` and `vizzutest` targets).

```
export CC=/usr/bin/gcc-12
export CXX=/usr/bin/g++-12
cmake -Dclangformat:BOOL="ON" ../../project/cmake/
cmake --build .
```

```
cmake -Dclangtidy:BOOL="ON" -Dcppcheck:BOOL="ON" ../../project/cmake/
cmake --build . --target cvizzu vizzutest
```

#### Building WASM version

NOTE: new terminal session needed after install, because emsdk environment setup runs on session begin from bashrc.

```
cd vizzu-lib
mkdir -p build/cmake-wasm
cd build/cmake-wasm
emcmake cmake ../../project/cmake/
cmake --build .
```

### Debugging

#### Debug WASM version under Chrome

- set Chrome/DevTools/Settings/Experiments/'WebAssembly Debugging: Enable DWARF support' to true
- set [repo]/project/cmake/emcc.txt: CMAKE_EXE_LINKER_FLAGS_DEBUG --source-map-base to the URL where the browser can find cvizzu.wasm.map file

### Building JS-API

Running prettier and eslint,
and creating the minified and bundled version:

```sh
cd project/js
npm install
npm run build
```

Note: You can build the js-api without building the `C++` part of the project.
Firstly, you need the download the actual versions of `cvizzu.wasm` and `cvizzu.js`:

```sh
cd project/js
npm run jsonly-download-cvizzu
```

After you can run prettier and eslint,
and create the minified and bundled version:

```sh
cd project/js
npm install
npm run jsonly-build
```
