# Unit testing

## CPP unit testing

Run cpp unit tests (in build folder):

```
ctest
```

CTest will write detailed output log to Testing/Temporary/LastTest.log.

Run unit tests (with detailed output log):

```
ctest -V
```

## JS unit testing

Run js unit tests (in test/unit folder):

```
npm install  # npm update
npm test
```

Select a single test suite:

```
npm test -t <test suite name>
```

Select a single test:

```
npm test -- -t <test name>
```

# Integration testing

For information on how integration testing works and what options it has, please see the program help:

```
cd test/integration
node test.js --help
```

## Test environment

It is recommended to use `Ubuntu 22.04` and setup the test environment with the steps below.

Install package dependencies:

```
sudo apt-get update
sudo apt-get install fonts-roboto fonts-noto-cjk gnupg wget curl unzip
```

It is recommended to use `Node.js 18`.
Install `nvm` and `Node.js 18`:

```
wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.3/install.sh | bash
export NVM_DIR="$([ -z "${XDG_CONFIG_HOME-}" ] && printf %s "${HOME}/.nvm" || printf %s "${XDG_CONFIG_HOME}/nvm")"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh" # This loads nvm

nvm install 18
nvm use default 18

node --version
```

Note: We use the latest `Chrome` and `Chromedriver` to keep our library up to date.
Install (or update) the latest `Chrome` and `Chromedriver`:

```
npm run chrome
```

Install `npm` package dependencies:

```
cd test/integration
npm install  # npm update
```

## Run all test cases

```
cd test/integration
node test.js
```

For more options please see the program help.

```
node test.js --help
```

## Manual testing

Test cases can be viewed using different versions of vizzu using the manual checker.

```
cd test/integration
node man.js
# Press CTRL and click on the URL to open it in the default browser
```

For more options please see the program help.

```
node man.js --help
```