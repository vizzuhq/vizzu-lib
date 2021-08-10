# Integration testing

# License

Copyright © 2021 [Vizzu Kft.](https://vizzuhq.com).

Released under the [Apache 2.0 License](LICENSE).

# Setting up and testing Vizzu on Ubuntu 20.04

## Install all test dependencies

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
sudo echo "deb http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list
sudo apt-get update
sudo apt-get install google-chrome-stable
CHROMEVER=$(google-chrome --product-version | grep -o "[^\.]*\.[^\.]*\.[^\.]*")
DRIVERVER=$(curl -s "https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$CHROMEVER")
[ -e /chromedriver/chromedriver ] && sudo rm /chromedriver/chromedriver
sudo wget -q --continue -P /chromedriver "http://chromedriver.storage.googleapis.com/$DRIVERVER/chromedriver_linux64.zip"
sudo unzip /chromedriver/chromedriver* -d /chromedriver
```

## Testing the project

### Getting the source code

```
cd $HOME
git clone git@github.com:vizzuhq/vizzu-lib.git
```

### Install npm dependencies:

```
cd $HOME/vizzu-lib/test/integration
npm install
```

### Testing

```
cd $HOME/vizzu-lib/test/integration
node vizzutest.js
```

For more information run:

```
cd $HOME/vizzu-lib/test/integration
node vizzutest.js -h
```