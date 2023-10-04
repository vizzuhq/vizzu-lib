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

The following steps demonstrate how to set up the development environment on an
`Ubuntu` `22.04` operating system. However, the process can be adapted for other
operating systems as well.

To contribute to the `JavaScript` part of the project, it is recommended to use
`Node.js` `18`.

Run the following command to install the `JavaScript` development dependencies:

```sh
npm run init-js
```

The `JavaScript` development requirements are installed based on the
`package-lock.json` file. To update the development requirements, you can use
the command `npm run lock-js`.

However, for the documentation we are also using `Python`. If you plan to
contribute to this part of the project, you will need `Python`, preferably
version `3.10`.

Run the following command to install the `Python` development dependencies:

```sh
npm run init-py
```

The `Python` development requirements are installed based on the
`tools/ci/pdm.lock` file. To update the development requirements, you can use
the command `npm run lock-py`.

Once set up, you can utilize the pre-defined `npm` scripts. For example, you can
initialize the `JavaScript` and `Python` development environments using the
command `npm run init`, or specific parts like `init-src`, `init-docs`, or
`init-tools`.

```sh
npm run init
```

**Note:** For all available `npm` scripts, run `npm run --list`.

To contribute to the `C++` part of the project, it is recommended to use
`Docker`, but based on the `Dockerfiles` below, you can also configure the
necessary dependencies on your local machine.

Run the following commands to build and run the `Desktop` version's development
environment

```sh
docker build -t vizzu/vizzu-dev-desktop -f tools/ci/docker/vizzu-dev-desktop .
docker run -i -t -v .:/workspace vizzu/vizzu-dev-desktop bash
```

or you can use a specific version of the prebuilt image:

```sh
docker run -i -t -v .:/workspace vizzu/vizzu-dev-desktop:0.8 bash
```

Run the following commands to build and run the `WASM` version's development
environment

```sh
docker build -t vizzu/vizzu-dev-wasm -f tools/ci/docker/vizzu-dev-wasm .
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm bash
```

or you can use a specific version of the prebuilt image:

```sh
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm:0.8 bash
```

### Building the project

#### Building Desktop version

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version and run the `C++` unit tests:

```sh
./tools/ci/pkg-build-desktop.sh
```

**Note:** A successful `gcc` and a `clang` build are required to contribute,
just like successful format checks and linter checks (on the `cvizzu` and
`vizzutest` targets).

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version with `gcc`, `clangformat`, and run the `C++` unit tests:

```sh
./tools/ci/pkg-build-desktop-clangformat.sh
```

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version with `clang`, `clangtidy`, `cppcheck` and run the `C++`
unit tests:

```sh
./tools/ci/pkg-build-desktop-clangtidy.sh
```

#### Building WASM version

Run the following script in the running `vizzu-dev-wasm` container to build the
`WASM` version, run the `C++` unit tests, check binary sizes, run `JavaScript`
unit tests, create `vizzu.min.js` and check `d.ts`:

```sh
./tools/ci/pkg-build-wasm.sh
```

**Note:** To debug WASM version under Chrome:

- set Chrome/DevTools/Settings/Experiments/'WebAssembly Debugging: Enable DWARF
  support' to true
- set \[repo\]/project/cmake/emcc.txt: CMAKE_EXE_LINKER_FLAGS_DEBUG
  --source-map-base to the URL where the browser can find cvizzu.wasm.map file.

#### Building npm package

If you used the above script to build the `WASM` version, the minified
`JavaScript` file is already created otherwise you can run
`npm run pkg-rollup-js`. After run the following command in order to create the
npm package:

```sh
npm run pkg-build-js
```

**Note:** This task will set the version number in the `package.json` file.

**Note:** You can build the `npm` package without building the `WASM` version:

```sh
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

Run the following command to start integration testing:

```sh
npm test
```

For information on how integration testing works and what options it has, please
see the program help:

```sh
npm test -- --help
```

#### Manual testing

Test cases can be viewed using different versions of `Vizzu` using the manual
checker.

```sh
npm run test-man
# Press CTRL and click on the URL to open it in the default browser
```

For more options please see the program help.

```sh
npm run test-man -- --help
```

### "Nightly" builds

Builds from the `CI` are available on the following `URLs`. However, you should
use these only for development purposes since they are subject to frequent and
sometimes unstable changes.

- build from the `HEAD` commit of the main branch:

  https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js

  and the minified, boundled version:

  https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.min.js

- all builds of the past 30 days:

  https://vizzu-lib-main-sha.storage.googleapis.com/lib-HASH/vizzu.js

  where `HASH` is the first 7 character of the commit's git hash.

### Documentation

You can read the online version at
[lib.vizzuhq.com](https://lib.vizzuhq.com/latest/).

### Release

`Vizzu` is distributed on [npm](https://www.npmjs.com/package/vizzu). **Note:**
You need to be an administrator to release the project.

To release `Vizzu`, follow the steps below:

- You should increase the version number in `src/chart/main/version.cpp`.

- If the major or minor version has changed, increase the version in
  `.github/workflows/docker-vizzu-dev-desktop.yml`,
  `.github/workflows/docker-vizzu-dev-wasm.yml` and `CONTRIBUTING.md`.

- Set the release and release date in `CHANGELOG.md`, under the `Unreleased`
  title.

- Create the release notes from `CHANGELOG.md` and publish the new release on
  [Releases](https://github.com/vizzuhq/vizzu-lib/releases).
