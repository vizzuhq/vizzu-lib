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

## Install all test dependencies on Ubuntu 20.04

Add Node.js to the repository list:

```
wget --quiet -O - https://deb.nodesource.com/setup_18.x | sudo bash
```

Install test dependencies:

```
sudo apt-get update
sudo apt-get install nodejs fonts-roboto fonts-noto-cjk gnupg wget curl unzip
```

Install or update latest chrome and chromedriver:

```
npm run chrome
```

## Testing the project

### Install NPM dependencies:

```
cd test/integration
npm install  # npm update
```

### Run all tests

```
cd test/integration
node test.js
# For more information run: node test.js -h
```

#### Manual testing

Test cases can be viewed using different versions of vizzu using the manual checker.\
Note: select version of Vizzu on the left (where HEAD is the latest stable)\
Note: select test case on the right

```
cd test/integration
node man.js
# Press CTRL and click on the URL to open it in the default browser
# For more information run: node man.js -h
```
