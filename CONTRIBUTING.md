# Contributing

## Issues

You can find our open issues in the project's
[issue tracker](https://github.com/vizzuhq/vizzu-lib/issues). Please let us know
if you find any issues or have any feature requests there.

## Contributing

If you want to contribute to the project, your help is very welcome. Just fork
the project, make your changes and send us a pull request. You can find the
detailed description of how to do this in
[Github's guide to contributing to projects](https://docs.github.com/en/get-started/quickstart/contributing-to-projects).

## CI-CD

### Development environment

For contributing to the project, it is recommended to use `Docker` for the `C++`
parts and `Node.js` `18` for the `JavaScript`. However, for the documentation we
are also using `Python`. If you plan to contribute to this part of the project,
you will need `Python`, preferably version `3.10`.

The following steps demonstrate how to set up the development environment on an
`Ubuntu` `22.04` operating system. However, the process can be adapted for other
operating systems as well.

To start using the `Vizzu` development environment, you need to install the
development dependencies.

```sh
npm run init-js
```

If you want to work with the documantation too, you need to set up the `Python`
development environment.

```sh
npm run init-py
```

Once set up, you can utilize the pre-defined `npm` scripts. For example, you can
initialize the entire development environment using the command `npm run init`,
or specific parts like `init-src`, `init-docs`, or `init-tools`.

```sh
npm run init
```

**Note:** For all available `npm` scripts, run `npm run --list`.

The development requirements are installed based on the `package-lock.json` and
`tools/ci/pdm.lock` files. To update the development requirements, you can use
the command `npm run lock`.

For better development practices, you can set up `pre-commit` and `pre-push`
hooks in your local `Git` repository.

### Building the project

#### Building Desktop version

Create and run a `Docker` container with the dependencies needed for the desktop
build

```
docker build -t vizzu/vizzu-dev-desktop -f tools/ci/docker/vizzu-dev-desktop .
docker run -i -t -v .:/workspace vizzu/vizzu-dev-desktop bash
```

or use a prebuild image:

```
docker run -i -t -v .:/workspace vizzu/vizzu-dev-desktop:0.8 bash
```

Run the following script to build and unit test the desktop version:

```
./tools/ci/pkg-build-desktop.sh
```

Note: A successful gcc and a clang build are required to contribute, just like
successful format checks and linter checks (on the `cvizzu` and `vizzutest`
targets).

```
./tools/ci/pkg-build-desktop-clangformat.sh
```

```
./tools/ci/pkg-build-desktop-clangtidy.sh
```

#### Building WASM version

Create and run a `Docker` container with the dependencies needed for the wasm
build

```
docker build -t vizzu/vizzu-dev-wasm -f tools/ci/docker/vizzu-dev-wasm .
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm bash
```

or use a prebuild image:

```
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm:0.8 bash
```

Run the following script to build and unit test the wasm version:

```
./tools/ci/pkg-build-wasm.sh
```

#### Debug WASM version under Chrome

- set Chrome/DevTools/Settings/Experiments/'WebAssembly Debugging: Enable DWARF
  support' to true
- set \[repo\]/project/cmake/emcc.txt: CMAKE_EXE_LINKER_FLAGS_DEBUG
  --source-map-base to the URL where the browser can find cvizzu.wasm.map file

#### Building JS-API

Run unit tests and create the minified and bundled version:

```sh
npm run test-unit-src
npm run pkg-rollup-js
npm run pkg-build-js
```

Note: You can build the `JS-API` without building the `C++` part of the project.

```sh
npm run test-unit-src
npm run pkg-rollup-jsonly
npm run pkg-build-js
```

### CI

The CI pipeline includes code formatting checks, code analysis, typing
validation, and unit tests for the `Vizzu` project.

To run the entire CI pipeline, execute the following `npm` script:

```sh
npm run ci
```

However, if you want to run the CI steps on specific parts of the project, you
can use the following scripts: `ci-src`, `ci-docs`, or `ci-tools`.

#### Formatting

You can check the code's formatting using the `format` script:

```sh
npm run format
```

If you need to fix any formatting issues, you can use the `fix-format` script:

```sh
npm run fix-format
```

If you wish to format specific parts of the project, you can use the following
scripts: `format-src`, `format-docs`, `format-tools`, or `fix-format-src`,
`fix-format-docs`, `fix-format-tools`.

#### Code analyses

To perform code analyses, you can use the `lint` script:

```sh
npm run lint
```

If you need to run code analyses for specific parts of the project, you can
utilize the following scripts: `lint-src`, `lint-docs`, or `lint-tools`.

#### Typing

For type checking, you can use the `type` script:

```sh
npm run type
```

If you want to check specific parts of the project, you can use the following
scripts: `type-src` or `type-tools`.

#### Testing

For information on how integration testing works and what options it has, please
see the program help:

```sh
npm test -- --help
```

#### Manual testing

Test cases can be viewed using different versions of `Vizzu` using the manual
checker.

```
npm run test-man
# Press CTRL and click on the URL to open it in the default browser
```

For more options please see the program help.

```
npm run test-man -- --help
```

### Documentation

You can read the online version at
[lib.vizzuhq.com](https://lib.vizzuhq.com/latest/).

### Release

`Vizzu` is distributed on [npm](https://www.npmjs.com/package/vizzu). **Note:**
You need to be an administrator to release the project.
