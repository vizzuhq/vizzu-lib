const path = require("path");
const fs = require("fs");

const TestEnv = require("../../modules/integration-test/test-env.js");
const Console = require("../../modules/console/console.js");

class TestConsole extends Console {
  #fileLog;

  #testSuiteLogPath;

  #testResultsLogsReady;

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
    }
    super(filePrefix, testSuiteLogPath);
    this.#fileLog = fileLog;
    this.#testSuiteLogPath = testSuiteLogPath;
    this.#testResultsLogsReady = this.#createResultsLogs();
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

  #createResultsLogs() {
    let resultsReadies = [];
    Object.entries(TestEnv.ResultLogTypes).forEach(([resultName, resultType]) => {
      let logPath = path.join(
        TestEnv.getTestSuiteReportPath(),
        resultType
      );
      resultsReadies.push(this.#createResultLog(logPath));
    });
    return Promise.all(resultsReadies);
  }

  #createResultLog(logPath) {
    return new Promise((resolve, reject) => {
      if (this.#fileLog) {
        fs.mkdir(path.dirname(logPath), { recursive: true }, (err) => {
          if (err) {
            console.error('Failed to create directory:', err);
            reject(err);
          } else {
            fs.writeFile(logPath, '', { flag: 'w' }, (err) => {
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

  #writeResultLog(logPath, line) {
    if (this.#fileLog) {
      this.#testResultsLogsReady.then(() => {
        return new Promise((resolve, reject) => {
          fs.writeFile(logPath, line, { flag: 'a' }, (err) => {
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

  writePassedLog(line) {
    const logPath = TestEnv.getTestSuitePassedLog()
    return this.#writeResultLog(logPath, line);
  }

  writeWarningsLog(line) {
    const logPath = TestEnv.getTestSuiteWarningsLog()
    return this.#writeResultLog(logPath, line);
  }

  writeFailedLog(line) {
    const logPath = TestEnv.getTestSuiteFailedLog()
    return this.#writeResultLog(logPath, line);
  }

  writeFailuresLog(line) {
    const logPath = TestEnv.getTestSuiteFailuresLog()
    return this.#writeResultLog(logPath, line);
  }
}

module.exports = TestConsole;
