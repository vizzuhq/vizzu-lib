#!/bin/bash

npm update

# wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -
# grep -qxF "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" /etc/apt/sources.list.d/google.list || echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" | sudo tee -a /etc/apt/sources.list.d/google.list
# sudo apt-get update
# sudo apt-get install google-chrome-stable
# CHROMEVER=$(google-chrome --product-version | grep -o "[^\.]*\.[^\.]*\.[^\.]*")

# DRIVERVER=$(curl -s "https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$CHROMEVER")
# [ -e /chromedriver ] && sudo rm -r /chromedriver/*
# sudo wget -q --continue -P /chromedriver "http://chromedriver.storage.googleapis.com/$DRIVERVER/chromedriver_linux64.zip"
# sudo unzip /chromedriver/chromedriver* -d /chromedriver

sudo mkdir -p /chromedriver
sudo chown -R $USER /chromedriver
sudo chmod 775 /chromedriver
wget --no-verbose -O /chromedriver/google-chrome-stable.deb https://dl.google.com/linux/chrome/deb/pool/main/g/google-chrome-stable/google-chrome-stable_114.0.5735.133-1_amd64.deb
sudo apt-get update -y && sudo apt-get install --allow-downgrades  -y /chromedriver/google-chrome-stable.deb
[ -e /chromedriver ] && rm -r /chromedriver/*
wget -q --continue -P /chromedriver "http://chromedriver.storage.googleapis.com/114.0.5735.90/chromedriver_linux64.zip"
unzip /chromedriver/chromedriver* -d /chromedriver
