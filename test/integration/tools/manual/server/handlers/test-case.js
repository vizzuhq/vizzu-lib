const fs = require("fs").promises;
const path = require("path");

const TestEnv = require("../../../../modules/integration-test/test-env.js");
const { TestHashStatus, TestCaseResult } = require("../../shared/test-case.js");

class TestCaseHandler {
  constructor(req, res) {
    this.req = req;
    this.res = res;
  }

  handle() {
    const { testCaseValue } = this.req.body;
    const testCase = JSON.parse(testCaseValue);
    new TestConfigUpdater(testCase).update()
      .then((status) => new TestLogUpdater(testCase, status).update()
        .then(() => {
          this.res.json({ status });
        })
      )
      .catch((err) => {
        console.error(err);
        this.res.status(500).send("internal server error");
      });
  }
}

class TestConfigUpdater {
  #testCase;
  #testCaseRelativeName;
  #testHashStatus;

  constructor(testCase) {
    this.#testCase = testCase;
    this.#testCaseRelativeName = this.#getTestCaseRelativeName();
  }

  #getTestCaseRelativeName() {
    const workspacePath = TestEnv.getWorkspacePath();
    return path.relative(
      path.join(workspacePath, this.#testCase.testSuite),
      path.join(workspacePath, this.#testCase.testName),
    );
  }

  update() {
    return new Promise((resolve, reject) => {
      if (this.#testCase.testResult === TestCaseResult.TYPES.PASSED || this.#testCase.testResult === TestCaseResult.TYPES.UNKNOWN) {
        this.#testHashStatus = TestHashStatus.TYPES.UNCHANGED;
        resolve(this.#testHashStatus);
      } else {
        this.#getNewHash()
          .then(newHash => this.#updateRefHash(newHash))
          .then(() => resolve(this.#testHashStatus))
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
      const newRefs = newConfig.test[this.#testCaseRelativeName]?.refs;
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
      const ref = refConfig.test[this.#testCaseRelativeName]?.refs[0];
      if (ref === newHash) {
        this.#testHashStatus = TestHashStatus.TYPES.UNCHANGED;
      } else {
        refConfig.test[this.#testCaseRelativeName].refs[0] = newHash;
        this.#testHashStatus = TestHashStatus.TYPES.UPDATED;
      }
    } else {
      ((refConfig.test ||= {})[this.#testCaseRelativeName] ||= {}).refs ||= [];
      refConfig.test[this.#testCaseRelativeName].refs[0] = newHash;
      this.#testHashStatus = TestHashStatus.TYPES.ADDED;
    }
    return refConfig;
  }

  #writeConfig(configPath, config) {
    const stringifiedConfig = JSON.stringify(config, null, 4);
    return fs.writeFile(configPath, stringifiedConfig)
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
    return fs.readFile(configPath, "utf-8")
      .then(config => {
        return JSON.parse(config);
      })
      .catch(error => {
        throw error;
      });
  }

  #getRefHash(refConfig) {
    try {
      const refs = refConfig.test[this.#testCaseRelativeName]?.refs;
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
    return path.relative(TestEnv.getTestSuiteRelativePath(), this.#testCase.testSuite);
  }
}

class TestLogUpdater {
  #testCase;
  #testCaseRelativeName;
  #testHashStatus;

  constructor(testCase, testHashStatus) {
    this.#testCase = testCase;
    this.#testCaseRelativeName = this.#getTestCaseRelativeName();
    this.#testHashStatus = testHashStatus;
  }

  #getTestCaseRelativeName() {
    return path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), this.#testCase.testName));
  }

  update() {
    if (this.#testHashStatus === TestHashStatus.TYPES.UPDATED) {
      const failedPath = TestEnv.getTestSuiteFailedLog();
      const passedPath = TestEnv.getTestSuitePassedLog();
      return Promise.all([
        this.#removeTestCaseFromLog(failedPath),
        this.#addTestCaseToLog(passedPath)
      ]);
    } else if (this.#testHashStatus === TestHashStatus.TYPES.ADDED) {
      const warningsPath = TestEnv.getTestSuiteWarningsLog();
      const passedPath = TestEnv.getTestSuitePassedLog();
      return Promise.all([
        this.#removeTestCaseFromLog(warningsPath),
        this.#addTestCaseToLog(passedPath)
      ]);
    } else {
      return Promise.resolve();
    }
  }

  #removeTestCaseFromLog(logPath) {
    return fs.readFile(logPath, "utf8")
      .then((data) => {
        const testCaseRelativeName = ` ${this.#testCaseRelativeName}`;
        const updatedData = data.replace(testCaseRelativeName, "");
        return fs.writeFile(logPath, updatedData, "utf8");
      })
      .catch((err) => {
        console.error(err);
      });
  }

  #addTestCaseToLog(logPath) {
    const testCaseRelativeName = ` ${this.#testCaseRelativeName}`;
    return fs.readFile(logPath, "utf8")
      .then((data) => {
        if (!data.includes(testCaseRelativeName)) {
          return fs.appendFile(logPath, testCaseRelativeName, "utf8");
        }
      })
      .catch((err) => {
        console.error(err);
      });
  }
}

module.exports = TestCaseHandler;
