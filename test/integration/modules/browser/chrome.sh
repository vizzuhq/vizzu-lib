#!/bin/bash

npm update

wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -
grep -qxF "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" /etc/apt/sources.list.d/google.list || echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" | sudo tee -a /etc/apt/sources.list.d/google.list
sudo apt-get update
sudo apt-get install google-chrome-stable
CHROMEVER=$(google-chrome --product-version | grep -o "[^\.]*\.[^\.]*\.[^\.]*")

DRIVERVER=$(curl -s "https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$CHROMEVER")
[ -e /chromedriver ] && sudo rm -r /chromedriver/*
sudo wget -q --continue -P /chromedriver "http://chromedriver.storage.googleapis.com/$DRIVERVER/chromedriver_linux64.zip"
sudo unzip /chromedriver/chromedriver* -d /chromedriver