const yargs = require("yargs");
const path = require("path");
const fs = require("fs");

const pLimitReady = import("p-limit");
const AggregateErrorReady = import("aggregate-error");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const BrowsersChrome = require("../../modules/browser/browsers-chrome.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const TestEnv = require("../../modules/integration-test/test-env.js");
const TestCasesConfig = require("../../modules/integration-test/test-case/test-cases-config.js");
const TestCases = require("../../modules/integration-test/test-case/test-cases.js");

function checkFileExist(path, timeout = 5000) {
  let totalTime = 0;
  let checkTime = 100;
  return new Promise((resolve, reject) => {
    const timer = setInterval(function () {
      totalTime += checkTime;
      let fileExists = fs.existsSync(path);
      if (fileExists || totalTime >= timeout) {
        clearInterval(timer);
        resolve(fileExists);
      }
    }, checkTime);
  });
}

class VideoRecorder {
  #pLimit;

  #browsersChrome;
  #browsersChromeReady;

  #vizzuUrl;
  #vizzuUrlReady;

  #reverse;

  #workspaceHost;
  #workspaceHostReady;
  #workspaceHostServerPort;

  #testCases;
  #testCasesConfigReady;
  #testCasesReady;

  constructor(configPathList, filters, browsersNum, vizzuUrl, reverse) {
    this.#browsersChrome = new BrowsersChrome(browsersNum, false);

    this.#vizzuUrl = vizzuUrl;
    this.#reverse = reverse;

    this.#testCasesConfigReady = TestCasesConfig.getConfig(configPathList);
    this.#testCasesReady = TestCases.getTestCases(
      this.#testCasesConfigReady,
      filters
    );
  }

  run() {
    return new Promise((resolve, reject) => {
      this.#runVideoRecorder()
        .catch((err) => {
          return reject(err);
        })
        .finally(() => {
          this.#destructVideoRecorder();
          return resolve();
        });
    });
  }

  #runVideoRecorder() {
    return new Promise((resolve, reject) => {
      fs.rmSync("generated", {
        force: true,
        recursive: true,
      });
      this.#testCasesReady.then((testCases) => {
        this.#testCases = testCases;
        if (testCases.filteredTestCases.length > 0) {
          this.#startVideoRecorder().then(() => {
            const limit = this.#pLimit.default(
              this.#browsersChrome.getBrowsersNum()
            );
            let testCasesReady = testCases.filteredTestCases.map(
              (filteredTestCase) => {
                return limit(() =>
                  this.#runVideoRecorderClient(filteredTestCase)
                );
              }
            );
            Promise.all(testCasesReady)
              .then(() => {
                return resolve();
              })
              .catch((err) => {
                return reject(err);
              });
          });
        }
      });
    });
  }

  #runVideoRecorderClient(testCase) {
    return new Promise((resolve, reject) => {
      if (testCase.errorMsg) return resolve();
      let browserChrome = this.#browsersChrome.shiftBrowser();
      let vizzuUrl = this.#vizzuUrl;
      if (vizzuUrl.startsWith("/")) {
        vizzuUrl = "/" + path.relative(TestEnv.getWorkspacePath(), vizzuUrl);
      }
      let suitePath =
        "/" +
        path.relative(TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
      let downloadedFile =
        path
          .relative(suitePath, path.dirname(testCase.testFile))
          .replaceAll("/", "___") +
        "___" +
        path.basename(testCase.testName) +
        ".webm";
      fs.rmSync(downloadedFile, {
        force: true,
      });
      let outputFile = "generated/" + downloadedFile.replaceAll("___", "/");
      browserChrome
        .getUrl(
          "http://127.0.0.1:" +
            String(this.#workspaceHostServerPort) +
            suitePath +
            "/modules/videorecorder/client/index.html" +
            "?testSuitePath=" +
            suitePath +
            "&testCasesPath=" +
            path.relative(suitePath, path.dirname(testCase.testFile)) +
            "&testCase=" +
            path.basename(testCase.testFile) +
            "&testName=" +
            path.basename(testCase.testName) +
            "&testType=" +
            testCase.testType +
            "&testIndex=" +
            testCase.testIndex +
            "&vizzuUrl=" +
            vizzuUrl + 
            "&reverse=" +
            this.#reverse
        )
        .then(() => {
          browserChrome
            .waitUntilTitleIs(
              "Finished",
              this.#browsersChrome.getTimeout() * 10
            )
            .then(() => {
              browserChrome.executeScript("return result").then((result) => {
                this.#browsersChrome.pushBrowser(browserChrome);
                if (result.result === "OK") {
                  console.log("OK:      " + testCase.testName);
                } else {
                  console.log(
                    "ERROR:   " + testCase.testName + " " + result.description
                  );
                }
                checkFileExist(downloadedFile).then((fileExists) => {
                  if (fileExists) {
                    fs.mkdirSync(path.dirname(outputFile), { recursive: true });
                    fs.renameSync(downloadedFile, outputFile);
                    return resolve(result);
                  } else {
                    return reject(
                      "TimeoutError: Waiting for file to be downloaded"
                    );
                  }
                });
              });
            })
            .catch((err) => {
              let errMsg = err.toString();
              if (
                !errMsg.includes(
                  'TimeoutError: Waiting for title to be "Finished"'
                )
              ) {
                this.#browsersChrome.pushBrowser(browserChrome);
                return reject(err);
              } else {
                this.#browsersChrome.pushBrowser(browserChrome);
                console.log("TIMEOUT: " + testCase);
                return resolve();
              }
            });
        });
    });
  }

  #startVideoRecorder() {
    return new Promise((resolve, reject) => {
      let startTestSuiteReady = [];

      startTestSuiteReady.push(pLimitReady);
      pLimitReady.then((pLimit) => {
        this.#pLimit = pLimit;
      });

      startTestSuiteReady.push(this.#testCasesConfigReady);

      startTestSuiteReady.push(this.#testCasesReady);

      this.#vizzuUrlReady = VizzuUrl.resolveVizzuUrl(
        this.#vizzuUrl,
        TestEnv.getWorkspacePath(),
        TestEnv.getTestSuitePath()
      );
      startTestSuiteReady.push(this.#vizzuUrlReady);
      this.#vizzuUrlReady.then((url) => {
        this.#vizzuUrl = url;
      });

      this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath());
      this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
      startTestSuiteReady.push(this.#workspaceHostReady);
      this.#workspaceHostReady.then((serverPort) => {
        this.#workspaceHostServerPort = serverPort;
      });

      this.#browsersChrome.setBrowsersNum(
        this.#testCases.filteredTestCases.length <
          this.#browsersChrome.getBrowsersNum()
          ? this.#testCases.filteredTestCases.length
          : this.#browsersChrome.getBrowsersNum()
      );
      this.#browsersChromeReady = this.#browsersChrome.startBrowsers();
      startTestSuiteReady.push(this.#browsersChromeReady);

      Promise.all(startTestSuiteReady)
        .then(() => {
          return resolve();
        })
        .catch((err) => {
          return reject(err);
        });
    });
  }

  #destructVideoRecorder() {
    AggregateErrorReady.then((AggregateError) => {
      let errs = [];
      try {
        this.#browsersChrome.closeBrowsers();
      } catch (err) {
        errs.push(err);
      }
      try {
        if (this.#workspaceHost) {
          this.#workspaceHost.closeServer();
        }
      } catch (err) {
        errs.push(err);
      }
      if (errs.length > 1) {
        throw new AggregateError(errs);
      } else if (errs.length == 1) {
        throw errs[0];
      }
    });
  }
}

try {
  var argv = yargs
    .help("h")
    .alias("h", "help")

    .version(false)

    .array("c")
    .alias("c", "configs")
    .nargs("c", 1)
    .describe(
      "c",
      "Change the list of config file's path of the test cases" +
        "\n(relative or absolute path where the repo folder is the root)" +
        "\n"
    )
    .default("c", [
      "/test/integration/test_cases/test_cases.json",
      "/test/integration/tests/style_tests.json",
    ])

    .string("vizzu")
    .nargs("vizzu", 1)
    .describe(
      "vizzu",
      "Change Vizzu url" +
        "\n(can be forced to use vizzu.js or vizzu.min.js if its given)" +
        '\n\n- "head": select the last stable Vizzu from the main branch' +
        "\n(default: vizzu.min.js)" +
        "\n\n- [sha]: select Vizzu with a short commit number" +
        "\n(default: vizzu.min.js)" +
        "\n\n- [version]: select Vizzu with a version number" +
        "\n(vizzu.min.js only)" +
        "\n\n- path: select Vizzu from the local file system" +
        "\n(relative or absolute path where the repo folder is the root)" +
        "\n(default: vizzu.js)" +
        "\n"
    )
    .default("vizzu", "/example/lib/vizzu.js")

    .number("b")
    .alias("b", "browsers")
    .describe("b", "Change number of parallel browser windows" + "\n")
    .default("b", 1)

    .boolean('r')
    .describe('r', 'Also run the steps in reverse order')
    .alias('r', 'reverse')
    .default('r', false)

    .example(
      "$0 ../../test_cases/web_content/animated/*",
      "Generate thumbnails for test_cases/web_content/animated"
    ).argv;

  let videoRecorder = new VideoRecorder(
    argv.configs,
    argv._,
    argv.browsers,
    argv.vizzu,
    argv.reverse
  );
  videoRecorder.run();
} catch (err) {
  process.exitCode = 1;
  console.error(err);
}
