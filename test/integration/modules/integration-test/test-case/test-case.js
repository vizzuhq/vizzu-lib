const path = require("path");

const VizzuUrl = require("../../../modules/vizzu/vizzu-url.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");
const TestCaseResult = require("../../../modules/integration-test/test-case/test-case-result.js");

class TestCase {
  static runTestCase(testCaseObj, vizzuUrl) {
    return new Promise((resolve, reject) => {
      let browserChrome = testCaseObj.browsersChrome.shiftBrowser();
      TestCase.runTestCaseClient(testCaseObj, browserChrome, vizzuUrl).then(
        (testData) => {
          testCaseObj.testSuiteResults.RESULTS[testCaseObj.testCase.testName] =
            testData;
          let testCaseResult = new TestCaseResult(
            testCaseObj,
            testData,
            browserChrome,
            vizzuUrl,
            TestCase.runTestCaseRef
          );
          testCaseResult.createTestCaseResult().then(() => {
            testCaseObj.browsersChrome.pushBrowser(browserChrome);
            return resolve();
          });
        }
      );
    });
  }

  static runTestCaseRef(testCaseObj, browserChrome) {
    return new Promise((resolve, reject) => {
      let vizzuUrl =
        VizzuUrl.getRemoteStableBucket() + "/lib" + VizzuUrl.getVizzuMinJs();
      TestCase.runTestCaseClient(testCaseObj, browserChrome, vizzuUrl)
        .then((testDataRef) => {
          return resolve(testDataRef);
        })
        .catch((err) => {
          return reject(err);
        });
    });
  }

  static runTestCaseClient(testCaseObj, browserChrome, vizzuUrl) {
    return new Promise((resolve, reject) => {
      let refHash = [];
      if (testCaseObj.testCase.testName in testCaseObj.testCasesConfig.tests) {
        if (
          "refs" in
          testCaseObj.testCasesConfig.tests[testCaseObj.testCase.testName]
        ) {
          refHash =
            testCaseObj.testCasesConfig.tests[testCaseObj.testCase.testName][
              "refs"
            ];
        }
      }
      if (vizzuUrl.startsWith("/")) {
        vizzuUrl = "/" + path.relative(TestEnv.getWorkspacePath(), vizzuUrl);
      }
      browserChrome
        .getUrl(
          "http://127.0.0.1:" +
            String(testCaseObj.workspaceHostServerPort) +
            "/test/integration/modules/integration-test/client/index.html" +
            "?testFile=" +
            testCaseObj.testCase.testFile +
            "&testType=" +
            testCaseObj.testCase.testType +
            "&testIndex=" +
            testCaseObj.testCase.testIndex +
            "&vizzuUrl=" +
            vizzuUrl +
            "&refHash=" +
            refHash.toString() +
            "&createImages=" +
            testCaseObj.createImages
        )
        .then(() => {
          browserChrome
            .waitUntilTitleIs("Finished", testCaseObj.animTimeout)
            .then(() => {
              browserChrome
                .executeScript("return testData")
                .then((testData) => {
                  return resolve(testData);
                });
            })
            .catch((err) => {
              if (
                !err
                  .toString()
                  .includes('TimeoutError: Waiting for title to be "Finished"')
              ) {
                throw err;
              }
              return resolve({ result: "ERROR", description: "Timeout" });
            });
        });
    });
  }
}

module.exports = TestCase;
