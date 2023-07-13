const fetch = require("node-fetch");
const fs = require("fs");
const path = require("path");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const VizzuCloudFunctions = require("../../modules/vizzu/vizzu-cloudfunctions.js");
const VizzuVersion = require("../../modules/vizzu/vizzu-version.js");
const TestEnv = require("../../modules/integration-test/test-env.js");
const TestCasesConfig = require("../../modules/integration-test/test-case/test-cases-config.js");
const TestCases = require("../../modules/integration-test/test-case/test-cases.js");
const { TestCaseResultUpdater } = require("../../modules/integration-test/test-case/test-case-result.js");

class Manual {
  #workspaceHost;
  #workspaceHostReady;
  #workspaceHostServerPort;

  #configPathList;
  #filters;

  constructor(configPathList, filters, workspaceHostServerPort) {
    this.#configPathList = configPathList;
    this.#filters = filters;
    this.#workspaceHostServerPort = workspaceHostServerPort;
  }

  run() {
    this.#workspaceHost = new WorkspaceHost(
      TestEnv.getWorkspacePath(),
      this.#workspaceHostServerPort
    );
    this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
    this.#workspaceHostReady.then(() => {
      this.#setRouteGetLibList();
      this.#setRouteGetTestList();
      this.#setRouteValidateTestCase();
      console.log(
        "[ W. HOST ] [ http://127.0.0.1:" +
          this.#workspaceHostServerPort +
          "/test/integration/modules/manual/client ] press CTRL + C to stop"
      );
    });
  }

  #setRouteGetLibList() {
    this.#workspaceHost.setRoute("/getLibList", (req, res) => {
      const localUrl = "http://127.0.0.1:" + this.#workspaceHostServerPort + "/example/lib/vizzu.js";
      const localReady = VizzuVersion.isUrlAvailable(localUrl).then((available) => {
        const versions = {};
        if (available) {
          versions["localhost"] = localUrl;
        } else {
          console.error("failed to fetch localhost");
        }
        return versions;
      });
      const headUrl = VizzuUrl.getRemoteStableBucket() + "/lib/vizzu.js";
      const headReady = VizzuVersion.isUrlAvailable(headUrl).then((available) => {
        const versions = {};
        if (available) {
          versions["HEAD"] = headUrl;
        } else {
          console.error("failed to fetch head");
        }
        return versions;
      });
      const shaListReady = this.#fetchLibList().then((vizzuList) => {
        const versions = {};
        vizzuList
          .slice()
          .reverse()
          .forEach((vizzu) => {
            versions[
              vizzu.time.substring(0, 10) +
                " " +
                vizzu.time.substring(11, 16) +
                " " +
                vizzu.sha
            ] = VizzuUrl.getRemoteBucket() + "/" + vizzu.sha;
          });
        return versions;
      });
      const cdnListReady = VizzuVersion.getPublicBetaList().then((cdnList) => {
        const versions = {};
        cdnList.forEach((version) => {
          versions[version.num] = version.url;
        });
        return versions;
      });

      Promise.all([localReady, headReady, shaListReady, cdnListReady]).then((versions) => {
        res.send(Object.assign({}, ...versions));
      }).catch((error) => {
        console.error(error);
        res.status(500).send("Internal Server Error");
      });
    });
  }

  #fetchLibList() {
    return new Promise((resolve) => {
      fetch(VizzuCloudFunctions.getRemoteCloudFunctions() + "/getVizzuList")
        .then((vizzuListUrl) => {
          vizzuListUrl
            .json()
            .then((vizzuList) => {
              resolve(vizzuList);
            })
            .catch((err) => {
              console.error(err);
              resolve([]);
            });
        })
        .catch((err) => {
          console.error("failed to fetch sha lib list");
          resolve([]);
        });
    });
  }

  #setRouteGetTestList() {
    this.#workspaceHost.setRoute("/getTestList", (req, res) => {
      let testCasesConfigReady = TestCasesConfig.getConfig(
        this.#configPathList
      );
      let testResultsReady = this.#getTestResults();
      let testCasesReady = TestCases.getTestCases(testCasesConfigReady, this.#filters);
      Promise.all([testCasesReady, testResultsReady]).then(
        (results) => {
          let testCases = results[0];
          let testResults = results[1];
          testCases.filteredTestCases.forEach(testCase => {
            testCase.testResult = this.#getTestCaseResult(testCase.testName, testResults);
          });
          res.send(testCases.filteredTestCases);
        }
      ).catch(error => {
        console.error(error);
        res.status(500).send("Internal Server Error");
      });
    });
  }

  #getTestCaseResult(testName, testResults) {
    const passed = testResults[0];
    const warnings = testResults[1];
    const failed = testResults[2];
    if (passed.includes(testName)) {
      return "PASS";
    } else if (warnings.includes(testName)) {
      return "WARN";
    } else if (failed.includes(testName)) {
      return "FAIL";
    }
  }

  #getTestResults() {
    const passed = this.#getPassed();
    const warnings = this.#getWarnings();
    const failed = this.#getFailed();
    return Promise.all([passed, warnings, failed]);
  }

  #getPassed() {
    const logPath = path.join(TestEnv.getTestSuiteReportPath(), "passed.log");
    return this.#getTestLog(logPath);
  }

  #getWarnings() {
    const logPath = path.join(TestEnv.getTestSuiteReportPath(), "warnings.log");
    return this.#getTestLog(logPath);
  }

  #getFailed() {
    const logPath = path.join(TestEnv.getTestSuiteReportPath(), "failed.log");
    return this.#getTestLog(logPath);
  }

  #getTestLog(logPath) {
    return new Promise((resolve) => {
      fs.readFile(logPath, "utf8", (err, data) => {
        if (err) {
          resolve([]);
        } else {
          const prefix = "/test/integration/";
          const tests = data
            .trim()
            .split(" ")
            .filter((test) => test !== "")
            .map((test) => prefix + test);
          resolve(tests);
        }
      });
    });
  }

  #setRouteValidateTestCase() {
    this.#workspaceHost.setPostRoute("/validateTestCase", (req, res) => {
      const { testCaseValue } = req.body;
      const testCase = JSON.parse(testCaseValue);
      const testCaseResultUpdater = new TestCaseResultUpdater(testCase);

      testCaseResultUpdater
        .update()
        .then((status) => {
          this.#updateTestLog(status, testCase)
            .then(() => {
              res.json({ message: status });
            })
            .catch((error) => {
              console.log(error);
              res.json({ message: "failed" });
            });
        })
        .catch((error) => {
          console.log(error);
          res.status(500).send("Internal Server Error");
        });
    });
  }

  #updateTestLog(status, testCase) {
    return new Promise((resolve, reject) => {
      const testName = path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), testCase.testName));
      if (status === "updated") {
        const failedPath = path.join(TestEnv.getTestSuiteReportPath(), "failed.log");
        const passedPath = path.join(TestEnv.getTestSuiteReportPath(), "passed.log");
  
        Promise.all([
          this.#removeTestCaseFromLog(testName, failedPath),
          this.#addTestCaseToLog(testName, passedPath)
        ])
          .then(() => resolve())
          .catch(error => reject(error));
      } else if (status === "added") {
        const warningsPath = path.join(TestEnv.getTestSuiteReportPath(), "warnings.log");
        const passedPath = path.join(TestEnv.getTestSuiteReportPath(), "passed.log");
  
        Promise.all([
          this.#removeTestCaseFromLog(testName, warningsPath),
          this.#addTestCaseToLog(testName, passedPath)
        ])
          .then(() => resolve())
          .catch(error => reject(error));
      } else {
        resolve();
      }
    });
  }  

  #removeTestCaseFromLog(testName, logPath) {
    return fs.promises.readFile(logPath, "utf8")
      .then(data => {
        const testCaseLog = " " + testName;
        const updatedData = data.replace(testCaseLog, "");
  
        return fs.promises.writeFile(logPath, updatedData, "utf8");
      })
      .catch(error => {
        console.error(error);
      });
  }
  
  #addTestCaseToLog(testName, logPath) {
    const testCaseLog = " " + testName;
    
    return fs.promises.readFile(logPath, "utf8")
      .then(data => {
        if (!data.includes(testCaseLog)) {
          return fs.promises.appendFile(logPath, testCaseLog, "utf8");
        }
      })
      .catch(error => {
        console.error(error);
      });
  }
  
}

module.exports = Manual;
