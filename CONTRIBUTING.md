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

Our [Roadmap page](https://github.com/vizzuhq/.github/wiki/Roadmap) is a
comprehensive list of tasks we want to do in the future. It is a good place to
start if you want to contribute to `Vizzu`. In case you have something else in
mind, that's awesome and we are very interested in hearing about it.

## CI-CD

### Development environment

The following steps demonstrate how to set up the development environment on an
`Ubuntu` `22.04` operating system. However, the process can be adapted for other
operating systems as well.

To contribute to the `JavaScript` part of the project, it is recommended to use
`Node.js` `18`.

Run the following command to install the `JavaScript` development dependencies:

```sh
npm install
```

The `JavaScript` development requirements are installed based on the
`package-lock.json` file. To update the development requirements, you can use
the command `npm run lock:js`.

However, for the documentation we are also using `Python`. If you plan to
contribute to this part of the project, you will need `Python`, preferably
version `3.10`.

Run the following command to install the `Python` development dependencies:

```sh
./tools/ci/run/init-py.sh
```

The `Python` development requirements are installed based on the
`tools/ci/pdm.lock` file. To update the development requirements, you can use
the command `npm run lock:py`.

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
docker run -i -t -v .:/workspace vizzu/vizzu-dev-desktop:0.10 bash
```

Run the following commands to build and run the `WASM` version's development
environment

```sh
docker build -t vizzu/vizzu-dev-wasm -f tools/ci/docker/vizzu-dev-wasm .
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm bash
```

or you can use a specific version of the prebuilt image:

```sh
docker run -i -t -v .:/workspace vizzu/vizzu-dev-wasm:0.10 bash
```

### Building the project

#### Building Desktop version

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version and run the `C++` unit tests:

```sh
./tools/ci/run/pkg-build-desktop.sh
```

**Note:** A successful `gcc` and a `clang` build are required to contribute,
just like successful format checks and linter checks (on the `cvizzu` and
`vizzutest` targets).

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version with `gcc`, `clangformat`, and run the `C++` unit tests:

```sh
./tools/ci/run/pkg-build-desktop-clangformat.sh
```

Run the following script in the running `vizzu-dev-desktop` container to build
the `Desktop` version with `clang`, `clangtidy`, `cppcheck` and run the `C++`
unit tests:

```sh
./tools/ci/run/pkg-build-desktop-clangtidy.sh
```

#### Building WASM version

Run the following script in the running `vizzu-dev-wasm` container to build the
`WASM` version, run the `C++` unit tests, check binary sizes, compile
`TypeScript`, run `JavaScript` unit tests, create `vizzu.min.js` and check
`d.ts`:

```sh
./tools/ci/run/pkg-build-wasm.sh
```

**Note:** To debug WASM version under Chrome:

- set Chrome/DevTools/Settings/Experiments/'WebAssembly Debugging: Enable DWARF
  support' to true
- set \[repo\]/project/cmake/emcc.txt: CMAKE_EXE_LINKER_FLAGS_DEBUG
  --source-map-base to the URL where the browser can find cvizzu.wasm.map file.

#### Building npm package

If you used the above script to build the `WASM` version, the minified
`JavaScript` file is already created otherwise you can run
`npm run build:ts && npm run rollup`. After run the following command in order
to create the npm package:

```sh
npm run set-version
npm run build:js
```

**Note:** This task will set the version number in the `package.json` file.

**Note:** You can build the `npm` package without building the `WASM` version:

```sh
npm run build:wasm-wocpp
npm run set-version
npm run build:js
```

### CI

The CI pipeline includes code formatting checks, code analysis, typing
validation, and unit tests for the `Vizzu` project.

To run the entire CI pipeline, execute the following `npm` script:

```sh
npm run ci
```

However, if you want to run the CI steps on specific parts of the project, you
can use the following scripts: `ci:src`, `ci:docs`, or `ci:tools`.

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
scripts: `format:src`, `format:docs`, `format:tools`, or `fix-format:src`,
`fix-format:docs`, `fix-format:tools`.

#### Code analyses

To perform code analyses, you can use the `lint` script:

```sh
npm run lint
```

If you need to run code analyses for specific parts of the project, you can
utilize the following scripts: `lint:src`, `lint:docs`, or `lint:tools`.

#### Typing

For type checking, you can use the `type` script:

```sh
npm run type
```

If you want to check specific parts of the project, you can use the following
script: `type:tools`.

#### Testing

Run the following command to start e2e testing:

```sh
npm test
```

For information on how e2e testing works and what options it has, please see the
program help:

```sh
npm test -- --help
```

#### Manual testing

Test cases can be viewed using different versions of `Vizzu` using the manual
checker.

```sh
npm run test:man
# Press CTRL and click on the URL to open it in the default browser
```

For more options please see the program help.

```sh
npm run test:man -- --help
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

**Note:** The showcases and some images are stored in the
[vizzu-lib-doc](https://github.com/vizzuhq/vizzu-lib-doc) repository. If you
wish to build the site, run the following command:

```sh
git clone --single-branch --branch main --depth 1 'git@github.com:vizzuhq/vizzu-lib-doc'
```

**Note:** If you also want to generate thumbnails, run the following command:

```sh
npm run gen-thumbnail
```

To build the documentation, you can use the `build-docs` script:

```sh
npm run build-docs
```

You can read the online version at
[lib.vizzuhq.com](https://lib.vizzuhq.com/latest/).

### Release

`Vizzu` is distributed on [npm](https://www.npmjs.com/package/vizzu). **Note:**
You need to be an administrator to release the project.

To release `Vizzu`, follow the steps below:

- You should increase the version number in `src/chart/main/version.cpp`.

- If the major or minor version has changed, increase the version in
  `.github/workflows/docker-vizzu-dev-desktop.yml`,
  `.github/workflows/docker-vizzu-dev-wasm.yml`,
  `tools/ci/gcp/cloudbuild/cloudbuild.yaml` and `CONTRIBUTING.md`.

- Set the release and release date in `CHANGELOG.md`, under the `Unreleased`
  title.

- Create the release notes from `CHANGELOG.md` and publish the new release on
  [Releases](https://github.com/vizzuhq/vizzu-lib/releases).
