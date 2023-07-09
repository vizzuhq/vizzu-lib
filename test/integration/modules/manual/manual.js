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
      console.log(
        "[ " +
          "W. HOST" +
          " ]" +
          " " +
          "[ " +
          "http://127.0.0.1:" +
          String(this.#workspaceHostServerPort) +
          "/test/integration/modules/manual/client" +
          " ]" +
          " press CTRL + C to stop"
      );
    });
  }

  #setRouteGetLibList() {
    this.#workspaceHost.setRoute("/getLibList", (req, res) => {
      let localUrl = "http://127.0.0.1:" + this.#workspaceHostServerPort + "/example/lib/vizzu.js";
      let localReady = VizzuVersion.isUrlAvailable(localUrl);
      localReady = localReady.then((available) => {
        let versions = {};
        if (available) {
          versions["localhost"] = localUrl;
        } else {
          console.error("failed to fetch localhost");
        }
        return versions;
      });
      let headUrl = VizzuUrl.getRemoteStableBucket() + "/lib/vizzu.js"
      let headReady = VizzuVersion.isUrlAvailable(headUrl);
      headReady = headReady.then((available) => {
        let versions = {};
        if (available) {
          versions["HEAD"] = headUrl;
        } else {
          console.error("failed to fetch head");
        }
        return versions;
      });
      let shaListReady = this.#fetchLibList();
      shaListReady = shaListReady.then((vizzuList) => {
        let versions = {};
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
      let cdnListReady = VizzuVersion.getPublicBetaList();
      cdnListReady = cdnListReady.then((cdnList) => {
        let versions = {};
        cdnList.forEach((version) => {
          versions[version.num] = version.url;
        });
        return versions;
      });
      Promise.all([localReady, headReady, shaListReady, cdnListReady]).then((versions) => {
        res.send(Object.assign({}, versions[0], versions[1], versions[2], versions[3]))
      })
    });
  }

  #fetchLibList() {
    return new Promise((resolve) => {
      fetch(
        VizzuCloudFunctions.getRemoteCloudFunctions() + "/getVizzuList"
      ).then((vizzuListUrl) => {
        vizzuListUrl
          .json()
          .then((vizzuList) => {
            return resolve(vizzuList);
          })
          .catch((err) => {
            console.error(err);
            return resolve([]);
          });
      }).catch((err) => {
        console.error("failed to fetch sha lib list");
        return resolve([]);
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
      );
    });
  }

  #getTestCaseResult(testName, testResults) {
    let passed = testResults[0];
    let warnings = testResults[1];
    let failed = testResults[2];
    if (passed.includes(testName)) {
      return "PASS";
    } else if (warnings.includes(testName)) {
      return "WARN"
    } else if (failed.includes(testName)) {
      return "FAIL";
    }
  }

  #getTestResults() {
    return Promise.all([
      this.#getPassed(),
      this.#getWarnings(),
      this.#getFailed(),
    ]);
  }

  #getPassed() {
    let logPath = path.join(
      TestEnv.getTestSuiteReportPath(),
      'passed.log'
    );
    return this.#getTestResult(logPath);
  }

  #getWarnings() {
    let logPath = path.join(
      TestEnv.getTestSuiteReportPath(),
      'warnings.log'
    );
    return this.#getTestResult(logPath);
  }

  #getFailed() {
    let logPath = path.join(
      TestEnv.getTestSuiteReportPath(),
      'failed.log'
    );
    return this.#getTestResult(logPath);
  }

  #getTestResult(logPath) {
    return new Promise((resolve, reject) => {
      fs.readFile(logPath, 'utf8', (err, data) => {
        if (err) {
          resolve([]);
        } else {
          const prefix = "/test/integration/";
          let tests = data
                        .trim()
                        .split(' ')
                        .filter((test) => test !== '')
                        .map((test) => prefix + test);
          resolve(tests);
        }
      });
    });
  }
}

module.exports = Manual;
