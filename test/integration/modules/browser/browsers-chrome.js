const path = require("path");
const fs = require("fs");

const Chrome = require("../../modules/browser/chrome.js");

class BrowsersChrome {
  #browsers = [];

  #browsersNum;
  #browsersGui;

  #browsersLogPath;
  #browsersLogFilePrefix;
  #browsersLogTimeStamp;

  #timeout = 120000;

  constructor(
    browsersNum,
    browsersGui,
    browsersLogPath = undefined,
    browsersLogTimeStamp = undefined
  ) {
    this.setBrowsersNum(browsersNum);
    this.#browsersGui = browsersGui;
    if (browsersLogPath && browsersLogTimeStamp) {
      this.#browsersLogPath = browsersLogPath;
      this.#browsersLogFilePrefix = "chrome";
      this.#browsersLogTimeStamp = browsersLogTimeStamp;
    } else {
      if (browsersLogPath || browsersLogTimeStamp) {
        throw new Error("parameter is required");
      }
    }
  }

  startBrowsers() {
    let browsersReady = [];
    for (let i = 0; i < this.#browsersNum; i++) {
      let browser = new Chrome(!this.#browsersGui);
      this.#browsers.push(browser);
      browsersReady.push(browser.initializing);
    }
    return Promise.all(browsersReady);
  }

  closeBrowsers() {
    this.#browsers.forEach((browser, index) => {
      if (browser) {
        let browserLogReady = new Promise((resolve) => {
          resolve();
        });
        if (this.#browsersLogPath) {
          browserLogReady = new Promise((resolve, reject) => {
            fs.mkdir(
              this.#browsersLogPath,
              { recursive: true, force: true },
              (err) => {
                if (err) {
                  return reject(err);
                }
                return resolve(
                  path.join(
                    this.#browsersLogPath,
                    this.#browsersLogFilePrefix +
                      "_" +
                      index +
                      "_" +
                      this.#browsersLogTimeStamp +
                      ".log"
                  )
                );
              }
            );
          });
        }
        browserLogReady.then((browserLog) => {
          browser.closeBrowser(browserLog);
        });
      }
    });
  }

  shiftBrowser() {
    return this.#browsers.shift();
  }

  pushBrowser(browser) {
    this.#browsers.push(browser);
  }

  getBrowsersNum() {
    return this.#browsersNum;
  }

  setBrowsersNum(browsersNum) {
    browsersNum = parseInt(browsersNum);
    if (isNaN(browsersNum)) {
      throw new Error("browsersNum is integer");
    }
    this.#browsersNum = browsersNum;

    let rate = 0.1;
    this.#timeout = parseInt(
      this.#timeout * Math.pow(1 + rate, this.#browsersNum)
    );
  }

  getTimeout() {
    return this.#timeout;
  }
}

module.exports = BrowsersChrome;
