const fetch = require("node-fetch");
const fs = require("fs").promises;
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
    this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath(), this.#workspaceHostServerPort);
    this.#workspaceHostReady = this.#workspaceHost.serverPortReady();

    this.#workspaceHostReady.then(() => {
      this.#setRouteGetLibList();
      this.#setRouteGetTestList();
      this.#setRouteValidateTestCase();

      console.log(`[ W. HOST ] [ http://127.0.0.1:${this.#workspaceHostServerPort}/test/integration/modules/manual/client ] press CTRL + C to stop`);
    });
  }

  #setRouteGetLibList() {
    this.#workspaceHost.setRoute("/getLibList", (req, res) => {
      const localUrl = `http://127.0.0.1:${this.#workspaceHostServerPort}/example/lib/vizzu.js`;
      const localReady = VizzuVersion.checkUrlAvailability(localUrl)
        .then(() => ({localhost: localUrl}))
        .catch((err) => {
          console.error(err);
          console.error("failed to fetch localhost");
          return {};
        });

      const headUrl = VizzuUrl.getRemoteStableBucket() + "/lib/vizzu.js";
      const headReady = VizzuVersion.checkUrlAvailability(headUrl)
        .then(() => ({HEAD: headUrl}))
        .catch((err) => {
          console.error(err);
          console.error("failed to fetch head");
          return {};
        });

      const shaListReady = this.#fetchLibList()
        .then((vizzuList) => {
          const versions = {};
          vizzuList.reverse().forEach((vizzu) => {
            versions[`${vizzu.time.substring(0, 10)} ${vizzu.time.substring(11, 16)} ${vizzu.sha}`] = VizzuUrl.getRemoteBucket() + "/" + vizzu.sha;
          });
          return versions;
        })
        .catch((err) => {
          console.error(err);
          console.error("failed to fetch sha lib list");
          return {};
        });

      const cdnListReady = VizzuVersion.getPublicBetaList()
        .then((cdnList) => {
          const versions = {};
          cdnList.forEach((version) => {
            versions[version.num] = version.url;
          });
          return versions;
        })
        .catch((err) => {
          console.error(err);
          console.error("failed to fetch public beta list");
          return {};
        });

      Promise.all([localReady, headReady, shaListReady, cdnListReady])
        .then((versions) => {
          res.send(Object.assign({}, ...versions));
        })
        .catch((err) => {
          console.error(err);
          res.status(500).send("internal server error");
        });
    });
  }

  #fetchLibList() {
    return fetch(VizzuCloudFunctions.getRemoteCloudFunctions() + "/getVizzuList")
      .then((vizzuListUrl) => vizzuListUrl.json())
      .catch((err) => {
        console.error(err);
        return [];
      });
  }

  #setRouteGetTestList() {
    this.#workspaceHost.setRoute("/getTestList", (req, res) => {
      const testCasesConfigReady = TestCasesConfig.getConfig(this.#configPathList);
      const testResultsReady = this.#getTestResults();
      const testCasesReady = TestCases.getTestCases(testCasesConfigReady, this.#filters);

      Promise.all([testCasesReady, testResultsReady])
        .then(([testCases, testResults]) => {
          testCases.filteredTestCases.forEach((testCase) => {
            testCase.testResult = this.#getTestCaseResult(testCase.testName, testResults);
          });
          res.send(testCases.filteredTestCases);
        })
        .catch((err) => {
          console.error(err);
          res.status(500).send("internal server error");
        });
    });
  }

  #getTestCaseResult(testName, testResults) {
    const [passed, warnings, failed] = testResults;
    if (passed.includes(testName)) {
      return "PASS";
    } else if (warnings.includes(testName)) {
      return "WARN";
    } else if (failed.includes(testName)) {
      return "FAIL";
    }
  }

  #getTestResults() {
    return Promise.all([
      this.#getPassed(),
      this.#getWarnings(),
      this.#getFailed()
    ]);
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
    return fs.readFile(logPath, "utf8")
      .then((data) => {
        const prefix = "/test/integration/";
        const tests = data.trim().split(" ")
          .filter((test) => test !== "")
          .map((test) => prefix + test);
        return tests;
      })
      .catch(() => []);
  }

  #setRouteValidateTestCase() {
    this.#workspaceHost.setPostRoute("/validateTestCase", (req, res) => {
      const { testCaseValue } = req.body;
      const testCase = JSON.parse(testCaseValue);
      const testCaseResultUpdater = new TestCaseResultUpdater(testCase);

      testCaseResultUpdater.update()
        .then((status) => this.#updateTestLog(status, testCase))
        .then(() => res.json({ message: status }))
        .catch((err) => {
          console.error(err);
          res.status(500).send("internal server error");
        });
    });
  }

  #updateTestLog(status, testCase) {
    const testName = path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), testCase.testName));

    if (status === "updated") {
      const failedPath = path.join(TestEnv.getTestSuiteReportPath(), "failed.log");
      const passedPath = path.join(TestEnv.getTestSuiteReportPath(), "passed.log");

      return Promise.all([
        this.#removeTestCaseFromLog(testName, failedPath),
        this.#addTestCaseToLog(testName, passedPath)
      ]);
    } else if (status === "added") {
      const warningsPath = path.join(TestEnv.getTestSuiteReportPath(), "warnings.log");
      const passedPath = path.join(TestEnv.getTestSuiteReportPath(), "passed.log");

      return Promise.all([
        this.#removeTestCaseFromLog(testName, warningsPath),
        this.#addTestCaseToLog(testName, passedPath)
      ]);
    } else {
      return Promise.resolve();
    }
  }

  #removeTestCaseFromLog(testName, logPath) {
    return fs.readFile(logPath, "utf8")
      .then((data) => {
        const testCaseLog = ` ${testName}`;
        const updatedData = data.replace(testCaseLog, "");

        return fs.writeFile(logPath, updatedData, "utf8");
      })
      .catch((err) => {
        console.error(err);
      });
  }

  #addTestCaseToLog(testName, logPath) {
    const testCaseLog = ` ${testName}`;

    return fs.readFile(logPath, "utf8")
      .then((data) => {
        if (!data.includes(testCaseLog)) {
          return fs.appendFile(logPath, testCaseLog, "utf8");
        }
      })
      .catch((err) => {
        console.error(err);
      });
  }
}

module.exports = Manual;
