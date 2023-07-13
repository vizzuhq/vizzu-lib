const pngjs = require("pngjs");
const pixelmatch = require("pixelmatch");

const path = require("path");
const fs = require("fs");

const TestEnv = require("../../../modules/integration-test/test-env.js");
const { error } = require("console");

class TestCaseResult {
  #cnsl;

  #testCaseObj;
  #testData;

  #browserChrome;
  #vizzuUrl;
  #vizzuRefUrl;

  #runTestCaseRef;

  constructor(testCaseObj, testData, browserChrome, vizzuUrl, vizzuRefUrl, runTestCaseRef) {
    this.#cnsl = testCaseObj.cnsl;

    this.#testCaseObj = testCaseObj;
    this.#testData = testData;
    this.#browserChrome = browserChrome;
    this.#vizzuUrl = vizzuUrl;
    this.#vizzuRefUrl = vizzuRefUrl;

    this.#runTestCaseRef = runTestCaseRef;
  }

  createTestCaseResult() {
    return new Promise((resolve, reject) => {
      let deleteTestCaseResultReady = new Promise((resolve) => {
        resolve();
      });
      if (this.#testCaseObj.createImages !== "DISABLED") {
        deleteTestCaseResultReady = this.#deleteTestCaseResult();
      }
      deleteTestCaseResultReady.then(() => {
        if (this.#testCaseObj.testCase.errorMsg) {
          if (this.#testData.result === "ERROR") {
            if (
              this.#testData.description.includes(
                this.#testCaseObj.testCase.errorMsg
              )
            ) {
              return resolve(
                this.#createTestCaseResultPassed(
                  this.#testCaseObj.testCase.errorMsg
                )
              );
            } else {
              return resolve(this.#createTestCaseResultError());
            }
          } else {
            this.#testData.result = "ERROR";
            this.#testData.description =
              "did not occur " + this.#testCaseObj.testCase.errorMsg;
            return resolve(this.#createTestCaseResultError());
          }
        } else {
          if (this.#testData.result === "PASSED") {
            return resolve(
              this.#createTestCaseResultPassed(this.#testData.hash)
            );
          } else if (this.#testData.result === "WARNING" || this.#testData.result === "FAILED") {
            return resolve(this.#createTestCaseResultFailure());
          } else {
            return resolve(this.#createTestCaseResultError());
          }
        }
      });
    });
  }

  #createTestCaseResultPassed(msg) {
    if (this.#testCaseObj.createImages === "ALL") {
      this.#createImage(this.#testData, "-1new");
    }
    this.#testCaseObj.testSuiteResults.PASSED.push(
      this.#testCaseObj.testCase.testName
    );
    this.#cnsl.writePassedLog(" " + this.#getFormattedTestName());
    this.#cnsl.log(
      ("[ " + "PASSED".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ] ")
        .success +
      "[ " +
      String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
        this.#cnsl.getTestNumberPad(),
        " "
      ) +
      " ] " +
      "[ " +
      msg +
      " ] " +
      path.relative(
        TestEnv.getTestSuitePath(),
        path.join(
          TestEnv.getWorkspacePath(),
          this.#testCaseObj.testCase.testName
        )
      )
    );
  }

  #createTestCaseResultFailure() {
    return new Promise((resolve, reject) => {
      if (this.#testCaseObj.createImages !== "DISABLED") {
        this.#createImage(this.#testData, "-1new");
      }
      if (this.#testData.result === "WARNING") {
        if (this.#testData.warning === "noref" && this.#testCaseObj.Werror.includes("noref")) {
          this.#createTestCaseResultFailed();
          return resolve();
        }
        this.#createTestCaseResultWarning();
        return resolve();
      } else {
        if (
          this.#testCaseObj.createImages !== "DISABLED" &&
          this.#vizzuRefUrl && this.#vizzuUrl !== this.#vizzuRefUrl
        ) {
          let testCaseObj = Object.assign({}, this.#testCaseObj);
          testCaseObj.createImages = "ALL";
          this.#runTestCaseRef(testCaseObj, this.#browserChrome, this.#vizzuRefUrl).then(
            (testDataRef) => {
              let failureMsgs = [];
              this.#createImage(testDataRef, "-2ref");
              this.#createDifImage(this.#testData, testDataRef);
              let diff = false;
              for (let i = 0; i < (this.#testData.hashes?.length ?? 0); i++) {
                for (let j = 0; j < (this.#testData.hashes?.[i]?.length ?? 0); j++) {
                  if (this.#testData.hashes[i][j] != testDataRef.hashes[i][j]) {
                    failureMsgs.push(
                      "".padEnd(this.#cnsl.getTestStatusPad() + 5, " ") +
                      "[ " +
                      "step: " +
                      i +
                      ". - seek: " +
                      this.#testData.seeks[i][j] +
                      " - hash: " +
                      this.#testData.hashes[i][j].substring(0, 7) +
                      " " +
                      "(ref: " +
                      testDataRef.hashes[i][j].substring(0, 7) +
                      ")" +
                      " ]"
                    );
                    diff = true;
                  }
                }
              }
              if (!diff) {
                failureMsgs.push(
                  "".padEnd(this.#cnsl.getTestStatusPad() + 5, " ") +
                  "[ the currently counted hashes are the same, the difference is probably caused by the environment ]"
                );
                this.#testData.warning = "sameref";
              }
              return failureMsgs;
            }
          ).then(failureMsgs => {
            if (this.#testData.warning === "sameref" && !this.#testCaseObj.Werror.includes("sameref")) {
              this.#createTestCaseResultWarning(failureMsgs);
              return resolve();
            }
            this.#createTestCaseResultFailed(failureMsgs);
            return resolve();
          });
        } else {
          this.#createTestCaseResultFailed();
          return resolve();
        }
      }
    });
  }

  #createTestCaseResultWarning(failureMsgs) {
    this.#testCaseObj.testSuiteResults.WARNING.push(
      this.#testCaseObj.testCase.testName
    );
    this.#cnsl.writeWarningsLog(" " + this.#getFormattedTestName());
    this.#createTestCaseResultManual();
    this.#cnsl.log(
      (
        "[ " +
        "WARNING".padEnd(this.#cnsl.getTestStatusPad(), " ") +
        " ] " +
        "[ " +
        String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
          this.#cnsl.getTestNumberPad(),
          " "
        ) +
        " ] " +
        "[ " +
        this.#testData.description +
        " ] "
      ).warn +
      path.relative(
        TestEnv.getTestSuitePath(),
        path.join(
          TestEnv.getWorkspacePath(),
          this.#testCaseObj.testCase.testName
        )
      )
    );
    if (failureMsgs) {
      failureMsgs.forEach(failureMsg => {
        this.#cnsl.log(failureMsg);
      });
    }
  }

  #createTestCaseResultFailed(failureMsgs) {
    this.#testCaseObj.testSuiteResults.FAILED.push(
      this.#testCaseObj.testCase.testName
    );
    this.#cnsl.writeFailedLog(" " + this.#getFormattedTestName());
    this.#createTestCaseResultManual();
    this.#createTestCaseResultErrorMsg();
    if (failureMsgs) {
      failureMsgs.forEach(failureMsg => {
        this.#cnsl.log(failureMsg);
      });
    }
  }

  #createTestCaseResultError() {
    this.#testCaseObj.testSuiteResults.FAILED.push(
      this.#testCaseObj.testCase.testName
    );
    this.#cnsl.writeFailedLog(" " + this.#getFormattedTestName());
    this.#createTestCaseResultManual();
    this.#createTestCaseResultErrorMsg();
  }

  #createTestCaseResultErrorMsg() {
    let errParts = this.#testData.description
      .split(
        "http://127.0.0.1:" + String(this.#testCaseObj.workspaceHostServerPort)
      )
      .join(path.resolve(TestEnv.getWorkspacePath()))
      .split("\n");
    this.#cnsl.log(
      (
        "[ " +
        this.#testData.result.padEnd(this.#cnsl.getTestStatusPad(), " ") +
        " ] " +
        "[ " +
        String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
          this.#cnsl.getTestNumberPad(),
          " "
        ) +
        " ] " +
        "[ " +
        errParts[0] +
        " ] "
      ).error +
      path.relative(
        TestEnv.getTestSuitePath(),
        path.join(
          TestEnv.getWorkspacePath(),
          this.#testCaseObj.testCase.testName
        )
      )
    );
    if (errParts.length > 1) {
      errParts.slice(1).forEach((item) => {
        this.#cnsl.log(
          "".padEnd(this.#cnsl.getTestStatusPad() + 7, " ") + item
        );
      });
    }
  }

  #createTestCaseResultManual() {
    this.#testCaseObj.testSuiteResults.MANUAL.push(this.#testCaseObj.testCase);
    let formatted = this.#getFormattedTestName()
    this.#testCaseObj.testSuiteResults.MANUAL_FORMATTED.push(formatted);
    this.#cnsl.writeFailuresLog(" " + formatted);
  }

  #deleteTestCaseResult() {
    return new Promise((resolve, reject) => {
      let testCaseResultPath = path.join(
        TestEnv.getTestSuiteResultsPath(),
        path.relative(
          TestEnv.getTestSuitePath(),
          path.join(
            TestEnv.getWorkspacePath(),
            this.#testCaseObj.testCase.testName
          )
        )
      );
      fs.rm(testCaseResultPath, { recursive: true, force: true }, (err) => {
        if (err) {
          return reject(err);
        }
        return resolve();
      });
    });
  }

  #getFormattedTestName() {
    return path.relative(
      TestEnv.getTestSuitePath(),
      path.join(TestEnv.getWorkspacePath(), this.#testCaseObj.testCase.testName)
    );
  }

  #createImage(data, fileAdd) {
    return new Promise((resolve, reject) => {
      let testCaseResultPath = path.join(
        TestEnv.getTestSuiteResultsPath(),
        path.relative(
          TestEnv.getTestSuitePath(),
          path.join(
            TestEnv.getWorkspacePath(),
            this.#testCaseObj.testCase.testName
          )
        )
      );
      fs.mkdir(testCaseResultPath, { recursive: true, force: true }, (err) => {
        if (err) {
          return reject(err);
        }
        for (let i = 0; i < (data.seeks?.length ?? 0); i++) {
          for (let j = 0; j < (data.seeks[i]?.length ?? 0); j++) {
            let seek = data.seeks[i][j].replace("%", "").split(".");
            if ((seek?.length ?? 0) == 1) {
              seek.push("0");
            }
            fs.writeFile(
              testCaseResultPath +
              "/" +
              path.basename(testCaseResultPath) +
              "_" +
              i.toString().padStart(3, "0") +
              "_" +
              seek[0].padStart(3, "0") +
              "." +
              seek[1].padEnd(3, "0") +
              "%" +
              fileAdd +
              ".png",
              data.images[i][j].substring(22),
              "base64",
              (err) => {
                if (err) {
                  return reject(err);
                }
                return resolve();
              }
            );
          }
        }
      });
    });
  }

  #createDifImage(testData, testDataRef) {
    let testCaseResultPath = path.join(
      TestEnv.getTestSuiteResultsPath(),
      path.relative(
        TestEnv.getTestSuitePath(),
        path.join(
          TestEnv.getWorkspacePath(),
          this.#testCaseObj.testCase.testName
        )
      )
    );
    for (let i = 0; i < (testData.seeks?.length ?? 0); i++) {
      for (let j = 0; j < (testData.seeks?.[i]?.length ?? 0); j++) {
        let seek = testData.seeks[i][j].replace("%", "").split(".");
        if ((seek?.length ?? 0) == 1) {
          seek.push("0");
        }
        const img1 = pngjs.PNG.sync.read(
          Buffer.from(testData.images[i][j].substring(22), "base64")
        );
        const img2 = pngjs.PNG.sync.read(
          Buffer.from(testDataRef.images[i][j].substring(22), "base64")
        );
        const { width, height } = img1;
        const diff = new pngjs.PNG({ width, height });
        const difference = pixelmatch(
          img1.data,
          img2.data,
          diff.data,
          width,
          height,
          { threshold: 0 }
        );
        if (difference) {
          fs.writeFile(
            testCaseResultPath +
            "/" +
            path.basename(testCaseResultPath) +
            "_" +
            i.toString().padStart(3, "0") +
            "_" +
            seek[0].padStart(3, "0") +
            "." +
            seek[1].padEnd(3, "0") +
            "%" +
            "-3diff" +
            ".png",
            pngjs.PNG.sync.write(diff),
            (err) => {
              if (err) {
                throw err;
              }
            }
          );
        }
      }
    }
  }
}

class TestCaseResultUpdater {
  #testCase;
  #relativeTestName;
  #status;

  constructor(testCaseValue) {
    this.#testCase = JSON.parse(testCaseValue);
    this.#relativeTestName = this.#getRelativeTestName();
  }

  #getRelativeTestName() {
    return path.relative(this.#testCase.testSuite, path.join(TestEnv.getWorkspacePath(), this.#testCase.testName));
  }

  update() {
    return new Promise((resolve, reject) => {
      if (this.#testCase.testResult === "PASS" || this.#testCase.testResult === "") {
        this.#status = "unchanged";
        resolve(this.#status);
      } else {
        this.#getNewHash()
          .then(newHash => this.#updateRefHash(newHash))
          .then(() => resolve(this.#status))
          .catch(error => reject(error));
      }
    });
  }

  #getNewHash() {
    return this.#loadNewHash()
      .catch(error => {
        console.log(error);
        throw new Error("Failed to get new hash");
      });
  }

  #updateRefHash(newHash) {
    const refConfigPath = this.#getRefConfigPath();
    return this.#getRefConfig()
      .then(refConfig => {
        refConfig = this.#addNewHash(refConfig, newHash);
        return this.#writeConfig(refConfigPath, refConfig);
      })
      .catch(error => {
        throw error;
      });
  }

  #loadNewHash() {
    const newConfigReady = this.#getNewConfig();
    return newConfigReady.then(newConfig => {
      const newRefs = newConfig.test[this.#relativeTestName]?.refs;
      if (newRefs?.length !== 1) throw new Error("No hash or multiple hashes are found");
      const newHash = newRefs[0];
      return newHash;
    });
  }

  #getRefConfigPath() {
    return this.#testCase.testConfig;
  }

  #getRefConfig() {
    const refConfigPath = this.#getRefConfigPath();
    return this.#loadConfig(refConfigPath);
  }

  #addNewHash(refConfig, newHash) {
    const refs = this.#getRefHash(refConfig);
    if (Array.isArray(refs)) {
      if (refs.length !== 1) throw new Error("No hash or multiple hashes are found");
      const ref = refConfig.test[this.#relativeTestName]?.refs[0];
      if (ref === newHash) {
        this.#status = "unchanged";
      } else {
        refConfig.test[this.#relativeTestName].refs[0] = newHash;
        this.#status = "updated";
      }
    } else {
      ((refConfig.test ||= {})[this.#relativeTestName] ||= {}).refs ||= [];
      refConfig.test[this.#relativeTestName].refs[0] = newHash;
      this.#status = "added";
    }
    return refConfig;
  }

  #writeConfig(configPath, config) {
    const stringifiedConfig = JSON.stringify(config, null, 4);
    return fs.promises
      .writeFile(configPath, stringifiedConfig)
      .then(() => {
        return;
      })
      .catch(error => {
        throw error;
      });
  }

  #getNewConfig() {
    const newConfigPath = this.#getNewConfigPath();
    return this.#loadConfig(newConfigPath);
  }

  #loadConfig(configPath) {
    return fs.promises
      .readFile(configPath, "utf-8")
      .then(config => {
        return JSON.parse(config);
      })
      .catch(error => {
        throw error;
      });
  }

  #getRefHash(refConfig) {
    try {
      const refs = refConfig.test[this.#relativeTestName]?.refs;
      return refs;
    } catch (error) {
      return;
    }
  }

  #getNewConfigPath() {
    const configName = path.basename(this.#testCase.testConfig);
    const relativeSuitePath = this.#getRelativeSuitePath();
    const configPath = path.join(TestEnv.getTestSuiteResultsPath(), relativeSuitePath, configName);
    return configPath;
  }

  #getRelativeSuitePath() {
    return path.relative(TestEnv.getTestSuitePath(), this.#testCase.testSuite);
  }
}

module.exports = { TestCaseResult, TestCaseResultUpdater };
