const path = require("path");
const fs = require("fs");

const TestEnv = require("../../modules/integration-test/test-env.js");
const Console = require("../../modules/console/console.js");

class TestConsole extends Console {
  #testSuiteLogPath;
  #testSuiteFailuresPath;
  #testSuiteFailuresPathReady;

  #testStatusPad = 8;
  #testNumberPad = 0;

  constructor(fileLog) {
    if (fileLog) {
      var filePrefix = "integration.test";
      var pathPrefix = "logs";
      var testSuiteLogPath = path.join(
        TestEnv.getTestSuiteReportPath(),
        pathPrefix
      );
      var testSuiteFailuresPath = path.join(
        TestEnv.getTestSuiteReportPath(),
        "failures.log"
      );
    }
    super(filePrefix, testSuiteLogPath);
    this.#testSuiteLogPath = testSuiteLogPath;
    this.#testSuiteFailuresPath = testSuiteFailuresPath;
    this.#testSuiteFailuresPathReady = this.#createFailuresLog();
  }

  getTestSuiteLogPath() {
    return this.#testSuiteLogPath;
  }

  getTestStatusPad() {
    return this.#testStatusPad;
  }

  getTestNumberPad() {
    return this.#testNumberPad;
  }

  setTestStatusPad(testStatusPad) {
    testStatusPad = parseInt(testStatusPad);
    if (isNaN(testStatusPad)) {
      throw new Error("testStatusPad is integer");
    }
    this.#testStatusPad = testStatusPad;
  }

  setTestNumberPad(testNumberPad) {
    testNumberPad = parseInt(testNumberPad);
    if (isNaN(testNumberPad)) {
      throw new Error("testNumberPad is integer");
    }
    this.#testNumberPad = testNumberPad;
  }

  #createFailuresLog() {
    return new Promise((resolve, reject) => {
      if (this.#testSuiteFailuresPath) {
        fs.mkdir(path.dirname(this.#testSuiteFailuresPath), { recursive: true }, (err) => {
          if (err) {
            console.error('Failed to create directory:', err);
            reject(err);
          } else {
            fs.writeFile(this.#testSuiteFailuresPath, '', { flag: 'w' }, (err) => {
              if (err) {
                console.error('Failed to write file:', err);
                reject(err);
              } else {
                resolve();
              }
            });
          }
        });
      } else {
        resolve();
      }
    });
  }

  writeFailure(line) {
    if (this.#testSuiteFailuresPath) {
      this.#testSuiteFailuresPathReady.then(() => {
        return new Promise((resolve, reject) => {
          fs.writeFile(this.#testSuiteFailuresPath, line, { flag: 'a' }, (err) => {
            if (err) {
              console.error('Failed to write file:', err);
              reject(err);
            } else {
              resolve();
            }
          });
        });
      });
    }
  }
}

module.exports = TestConsole;
