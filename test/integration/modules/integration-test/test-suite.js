const path = require("path");
const fs = require("fs");

const pLimitReady = import("p-limit");
const AggregateErrorReady = import("aggregate-error");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const BrowsersChrome = require("../../modules/browser/browsers-chrome.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const TestEnv = require("../../modules/integration-test/test-env.js");
const TestConsole = require("../../modules/integration-test/test-console.js");
const TestCase = require("../../modules/integration-test/test-case/test-case.js");
const TestCasesConfig = require("../../modules/integration-test/test-case/test-cases-config.js");
const TestCases = require("../../modules/integration-test/test-case/test-cases.js");
const TestSuiteResult = require("../../modules/integration-test/test-suite-result.js");


class TestSuite {

    #cnsl;
    #pLimit;

    #browsersChrome;
    #browsersChromeReady;

    #vizzuUrl;
    #vizzuUrlReady;

    #workspaceHost;
    #workspaceHostReady;
    #workspaceHostServerPort;

    #createImages;
    #createHashes;

    #testCasesConfig
    #testCasesConfigReady;
    #testCases;
    #testCasesReady;

    #testSuiteResults = {
        PASSED: [],
        WARNING: [],
        FAILED: [],
        TIME: { START: Math.round(Date.now() / 1000), END: 0 },
        FINISHED: 0,
        MANUAL: [],
        RESULTS: {}
    };


    constructor(
        configPathList,
        filters,
        noLogs,
        browsersNum,
        browserGui,
        vizzuUrl,
        createImages,
        createHashes
    ) {

        this.#cnsl = new TestConsole(!noLogs);
        this.#browsersChrome = new BrowsersChrome(browsersNum,
            browserGui,
            this.#cnsl.getTestSuiteLogPath(),
            ((this.#cnsl.getTestSuiteLogPath()) ?
                this.#cnsl.getTimeStamp() :
                this.#cnsl.getTestSuiteLogPath()));

        this.#vizzuUrl = vizzuUrl;

        this.#createImages = createImages;
        this.#createHashes = createHashes;

        this.#testCasesConfigReady = TestCasesConfig.getConfig(configPathList);
        this.#testCasesReady = TestCases.getTestCases(this.#testCasesConfigReady, filters);
    }


    static del() {
        fs.rm(TestEnv.getTestSuiteReportPath(), { recursive: true, force: true }, err => {
            if (err) {
                throw err;
            }
        });
    }


    test() {
        return new Promise((resolve, reject) => {
            this.#runTestSuite().catch(err => {
                return reject(err);
            }).finally(() => {
                this.#destructTestSuite();
                return resolve();
            });
        });
    }


    cnsl() {
        return this.#cnsl;
    }


    #runTestSuite() {
        return new Promise((resolve, reject) => {
            this.#testCasesReady.then(testCases => {
                this.#testCases = testCases;
                if (testCases.filteredTestCases.length > 0) {
                    this.#startTestSuite().then(() => {
                        this.#cnsl.setTestNumberPad(String(testCases.filteredTestCases.length).length);
                        const limit = this.#pLimit.default(this.#browsersChrome.getBrowsersNum());
                        let testCasesReady = testCases.filteredTestCases.map(filteredTestCase => {
                            let testCaseObj = {
                                testCase: filteredTestCase,
                                testCasesConfig: this.#testCasesConfig,
                                testSuiteResults: this.#testSuiteResults,
                                workspaceHostServerPort: this.#workspaceHostServerPort,
                                browsersChrome: this.#browsersChrome,
                                createImages: this.#createImages,
                                animTimeout: this.#browsersChrome.getTimeout(),
                                cnsl: this.#cnsl
                            };
                            return limit(() => TestCase.runTestCase(testCaseObj, this.#vizzuUrl));
                        });
                        Promise.all(testCasesReady).then(() => {
                            let testSuiteResult = new TestSuiteResult(
                                this.#testSuiteResults, 
                                this.#testCases, 
                                this.#testCasesConfig, 
                                this.#createHashes, 
                                this.#cnsl
                            );
                            testSuiteResult.createTestSuiteResult();
                            return resolve();
                        }).catch(err => {
                            return reject(err);
                        });
                    });
                }
            });
        });
    }


    #startTestSuite() {
        return new Promise((resolve, reject) => {
            let startTestSuiteReady = [];
            
            this.#cnsl.log("[ " + "LOG TIME".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#cnsl.getTimeStamp() + " ]");
            
            startTestSuiteReady.push(pLimitReady);
            pLimitReady.then(pLimit => {
                this.#pLimit = pLimit;
            });
            
            startTestSuiteReady.push(this.#testCasesConfigReady);
            this.#testCasesConfigReady.then(testCasesConfig => {
                this.#testCasesConfig = testCasesConfig;
                testCasesConfig.suites.forEach(suite => {
                    this.#cnsl.log("[ " + "T.CONFIG".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + suite.config + " ]");
                    this.#cnsl.log("[ " + "T.CASES".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + suite.suite + " ]");
                });
            });

            startTestSuiteReady.push(this.#testCasesReady);
            this.#testCasesReady.then(testCases => {
                this.#cnsl.log("[ " + "T.CASES".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + testCases.filteredTestCases.length + " / " + testCases.testCases.length + " ]");
            });

            this.#vizzuUrlReady = VizzuUrl.resolveVizzuUrl(this.#vizzuUrl, TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
            startTestSuiteReady.push(this.#vizzuUrlReady);
            this.#vizzuUrlReady.then(url => {
                this.#vizzuUrl = url;
                this.#cnsl.log("[ " + "V. URL".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + url + " ]");
            });

            this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath());
            this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
            startTestSuiteReady.push(this.#workspaceHostReady);
            this.#workspaceHostReady.then(serverPort => {
                this.#workspaceHostServerPort = serverPort;
                this.#cnsl.log("[ " + "W. HOST".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + "http://127.0.0.1:" + String(serverPort) + " ]");
            });

            this.#browsersChrome.setBrowsersNum(((this.#testCases.filteredTestCases.length < this.#browsersChrome.getBrowsersNum()) ?
                this.#testCases.filteredTestCases.length :
                this.#browsersChrome.getBrowsersNum()));
            this.#cnsl.log("[ " + "BROWSERS".padEnd(this.#cnsl.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#browsersChrome.getBrowsersNum() + " ]");
            this.#browsersChromeReady = this.#browsersChrome.startBrowsers();
            startTestSuiteReady.push(this.#browsersChromeReady);

            Promise.all(startTestSuiteReady).then(() => {
                return resolve();
            }).catch(err => {
                return reject(err);
            });
        });
    }


    #destructTestSuite() {
        AggregateErrorReady.then(AggregateError => {
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


module.exports = TestSuite;
