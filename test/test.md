# Unit testing

Run unit tests (in build folder):

```
ctest
```

CTest will write detailed output log to Testing/Temporary/LastTest.log.

Run unit tests (with detailed output log):

```
ctest -V
```

# Integration testing

## Install all test dependencies on Ubuntu 20.04

Add Node.js to the repository list:

```
wget --quiet -O - https://deb.nodesource.com/setup_14.x | sudo bash
```

Install test dependencies:

```
sudo apt-get install nodejs fonts-roboto fonts-noto-cjk gnupg wget curl unzip
```

Install latest chrome and chromedriver:

```
wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -
grep -qxF "deb http://dl.google.com/linux/chrome/deb/ stable main" /etc/apt/sources.list.d/google.list || echo "deb http://dl.google.com/linux/chrome/deb/ stable main" | sudo tee -a /etc/apt/sources.list.d/google.list
sudo apt-get update
sudo apt-get install google-chrome-stable
CHROMEVER=$(google-chrome --product-version | grep -o "[^\.]*\.[^\.]*\.[^\.]*")
DRIVERVER=$(curl -s "https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$CHROMEVER")
[ -e /chromedriver/chromedriver ] && sudo rm /chromedriver/chromedriver
sudo wget -q --continue -P /chromedriver "http://chromedriver.storage.googleapis.com/$DRIVERVER/chromedriver_linux64.zip"
sudo unzip /chromedriver/chromedriver* -d /chromedriver
```

## Testing the project

### Install NPM dependencies:

```
cd test/integration
npm install
```

### Run all tests

```
cd test/integration
node vizzutest.js
```

For more information run:

```
cd test/integration
node vizzutest.js -h
```

#### Manual testing

Test cases can be viewed using different versions of vizzu using the manual checker.\
Note: select version of Vizzu on the left (where HEAD is the latest stable)\
Note: select test case on the right

```
cd test/integration/manual
node manual.js
# Press CTRL and click on the URL to open it in the default browser
```
