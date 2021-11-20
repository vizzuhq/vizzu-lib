const path = require("path");
const fs = require("fs");

const pLimitReady = import("p-limit");
const AggregateErrorReady = import("aggregate-error");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const WorkspacePath = require("../../modules/workspace/workspace-path.js");
const Chrome = require("../../modules/browser/chrome.js");
const VizzuUrl = require("../../modules/integration-test/vizzu-url.js");
const TestEnv = require("../../modules/integration-test/test-env.js");
const TestConsole = require("../../modules/integration-test/test-console.js");
const TestCaseResult = require("../../modules/integration-test/test-case/test-case-result.js");


class TestSuite {

    #testConsole;

    #animStep = "20%";
    #animTimeout;

    #cfgVizzuUrl;
    #vizzuUrlReady;
    #vizzuUrl;

    #startTestSuiteReady = [];
    #testSuiteResults = { PASSED: [], WARNING: [], FAILED: [], TIME: { START: Math.round(Date.now() / 1000), END: 0 }, FINISHED: 0 };

    #cfgTestCasesHashListPath;
    #testCasesHashListPath;
    #testCasesHashListReady;
    #testCasesHashList;
    
    #cfgTestCasesFilters
    #filteredTestCasesReady;
    #filteredTestCases;

    #cfgTestCasesPath;
    #testCasesPath;
    #testCasesReady;
    #testCases = [];
    
    #testCasesResults = {};
    
    #workspaceHost;
    #workspaceHostReady;
    #workspaceHostServerPort;

    #cfgBrowserGui;
    #cfgBrowsersNum;
    #browsersNum;
    #browsersList = [];
    #browsersReady = [];

    #cfgNoLogs;
    #cfgCreateImages;
    #cfgCreateHashes;


    constructor(cfgTestCasesPath, 
                cfgTestCasesHashListPath, 
                cfgTestCasesFilters, 
                cfgVizzuUrl, 
                cfgBrowserGui, 
                cfgBrowsersNum, 
                cfgNoLogs, 
                cfgCreateImages, 
                cfgCreateHashes) {

        this.#testConsole = new TestConsole(!cfgNoLogs);

        this.#cfgTestCasesPath = cfgTestCasesPath;
        this.#cfgTestCasesHashListPath = cfgTestCasesHashListPath;
        this.#cfgTestCasesFilters = cfgTestCasesFilters;
        this.#cfgVizzuUrl = cfgVizzuUrl;
        this.#cfgBrowserGui = cfgBrowserGui;
        this.#cfgBrowsersNum = cfgBrowsersNum;
        this.#cfgNoLogs = cfgNoLogs;
        this.#cfgCreateImages = cfgCreateImages;
        this.#cfgCreateHashes = cfgCreateHashes;

        this.#testConsole.log("[ " + "LOG TIME".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#testConsole.getTimeStamp() + " ]");
        this.#testCasesHashListPath = WorkspacePath.resolvePath(this.#cfgTestCasesHashListPath, TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
        this.#testConsole.log("[ " + "T.HASHES".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#testCasesHashListPath + " ]");
        this.#testCasesHashListReady = this.#setTestCasesHashList();
        this.#testCasesPath = WorkspacePath.resolvePath(this.#cfgTestCasesPath, TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
        this.#testConsole.log("[ " + "T.CASES".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#testCasesPath + " ]");
        this.#testCasesReady = this.#setTestCases(this.#testCasesPath);
        this.#filteredTestCasesReady = this.#filterTestCases();
        Promise.all([this.#filteredTestCasesReady, this.#testCasesReady]).then(([filteredTestCases, testCases]) => {
            this.#testConsole.log("[ " + "T.CASES".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + this.#filteredTestCases.length + " / " + this.#testCases.length + " ]");
        });
        this.#vizzuUrlReady = VizzuUrl.resolveVizzuUrl(this.#cfgVizzuUrl, TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
        this.#vizzuUrlReady.then(url => {
            this.#vizzuUrl = url;
            this.#testConsole.log("[ " + "V. URL".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + url + " ]");
        });
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
        return this.#testConsole;
    }


    #runTestSuite() {
        return new Promise((resolve, reject) => {
            this.#filteredTestCasesReady.then(() => {
                if (this.#filteredTestCases.length > 0) {
                    this.#startTestSuite().then(pLimit => {
                        const limit = pLimit.default(this.#browsersNum);
                        let testCasesReady = this.#filteredTestCases.map(testCase => {
                            return limit(() => this.#runTestCase(testCase));
                        });
                        Promise.all(testCasesReady).then(() => {
                            this.#createTestSuiteResults();
                            this.#createHashes();
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
            this.#startTestSuiteReady.push(pLimitReady);
            this.#startTestSuiteReady.push(this.#testCasesHashListReady);
            this.#startTestSuiteReady.push(this.#vizzuUrlReady);

            this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath());
            this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
            this.#workspaceHostReady.then(serverPort => {
                this.#workspaceHostServerPort = serverPort;
                this.#testConsole.log("[ " + "W. HOST".padEnd(this.#testConsole.getTestStatusPad(), " ") + " ]" + " " + "[ " + "http://127.0.0.1:" + String(serverPort) + " ]");
            });
            this.#startTestSuiteReady.push(this.#workspaceHostReady);

            this.#setBrowserNumber();
            for (let i = 0; i < this.#browsersNum; i++) {
                let browser = new Chrome(!this.#cfgBrowserGui);
                this.#browsersList.push(browser);
                this.#startTestSuiteReady.push(browser.initializing);           
            }

            this.#setAnimTimeout();
            
            Promise.all(this.#startTestSuiteReady).then(([ pLimit ]) => {
                return resolve(pLimit);
            }).catch(err => {
                return reject(err);
            });
        });
    }


    #runTestCase(testCase) {
        return new Promise((resolve, reject) => {
            let browser = this.#browsersList.shift();
            this.#runTestCaseClient(testCase, this.#vizzuUrl, browser).then(testData => {
                this.#testCasesResults[testCase] = testData;
                let testCaseResult = new TestCaseResult(this.#testConsole,
                                                        testCase,
                                                        testData,
                                                        this,
                                                        this.#testSuiteResults,
                                                        browser,
                                                        this.#vizzuUrl,
                                                        this.#workspaceHostServerPort,
                                                        this.#cfgCreateImages,
                                                        String(this.#filteredTestCases.length).length);
                testCaseResult.createTestCaseResult().then((result) => {
                    this.#browsersList.push(browser);
                    return resolve();
                });
            });
        });
    }


    runTestCaseRef(testCase, browser) {
        return new Promise((resolve, reject) => {
            let vizzuUrl = VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuMinJs();
            this.#runTestCaseClient(testCase, vizzuUrl, browser).then(testDataRef => {
                return resolve(testDataRef);
            }).catch(err => {
                return reject(err);
            });
        });
    }


    #runTestCaseClient(testCase, vizzuUrl, browser) {
        return new Promise((resolve, reject) => {
            let animstep = this.#animStep;
            let refHash = [];
            if (testCase in this.#testCasesHashList) {
                if ("animstep" in this.#testCasesHashList[testCase]) {
                    animstep = this.#testCasesHashList[testCase]["animstep"].replace("%", "");
                }
                if ("refs" in this.#testCasesHashList[testCase]) {
                    refHash = this.#testCasesHashList[testCase]["refs"];
                }
            }
            if (vizzuUrl.startsWith("/")) {
                vizzuUrl = "/" + path.relative(TestEnv.getWorkspacePath(), vizzuUrl);
            }
            browser.getUrl("http://127.0.0.1:" + String(this.#workspaceHostServerPort)
                + "/test/integration/modules/integration-test-client/index.html"
                + "?testCasesPath=" + path.relative(TestEnv.getWorkspacePath(), this.#testCasesPath)
                + "&testCase=" + testCase
                + "&vizzuUrl=" + vizzuUrl
                + "&animstep=" + animstep
                + "&refHash=" + refHash.toString()
                + "&createImages=" + this.#cfgCreateImages)
            .then(() => {
                browser.waitUntilTitleIs("Finished", this.#animTimeout).then(() => {
                    browser.executeScript("return testData").then(testData => {
                        return resolve(testData);
                    })
                }).catch(err => {
                    let errMsg = err.toString();
                    if (!errMsg.includes("TimeoutError: Waiting for title to be \"Finished\"")) {
                        throw err;
                    }
                    return resolve({ result: "ERROR", description: "Timeout" });
                });
            });
        });
    }


    #createTestSuiteResults() {
        this.#testSuiteResults.TIME.END = Math.round(Date.now() / 1000);
        let duration = this.#testSuiteResults.TIME.END - this.#testSuiteResults.TIME.START;
        this.#testConsole.log("\n" + "duration:".padEnd(15, " ") + duration + "s");
        this.#testConsole.log("\n" + "all tests:".padEnd(15, " ") + this.#testCases.length);
        const sum = this.#testSuiteResults.PASSED.length + this.#testSuiteResults.WARNING.length + this.#testSuiteResults.FAILED.length;
        this.#testConsole.log("tests run:".padEnd(15, " ") + sum);
        this.#testConsole.log(("tests passed:".padEnd(15, " ") + this.#testSuiteResults.PASSED.length).success);
        if (this.#testSuiteResults.WARNING.length != 0) {
            this.#testConsole.log(("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length).warn);
        } else {
            this.#testConsole.log("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length);
        }
        if (this.#testSuiteResults.FAILED.length != 0) {
            this.#testConsole.log(("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length).error);
            process.exitCode = 1;
            this.#testSuiteResults.FAILED.forEach(testCase => {
                this.#testConsole.log("".padEnd(this.#testConsole.getTestStatusPad() + 5, " ") + testCase + " http://127.0.0.1:8080/test/integration/manual/?version=localhost&testCase=" + testCase);
            });
        } else {
            this.#testConsole.log("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length);
        }
    }


    #destructTestSuite() {
        AggregateErrorReady.then(AggregateError => {
            let errs = [];
            try {
                this.#browsersList.forEach((browser, index) => {
                    if (browser) {
                        let browserLogReady = new Promise(resolve => {resolve()});
                        if (!this.#cfgNoLogs) {
                            browserLogReady = new Promise((resolve, reject) => {
                                let logPath = this.#testConsole.getTestSuiteLogPath();
                                fs.mkdir(logPath, { recursive: true, force: true }, err => {
                                    if (err) {
                                        return reject(err);
                                    }
                                    return resolve(path.join(logPath, "chromedriver_" + index  + '_' + this.#testConsole.getTimeStamp() + ".log"));
                                });
                            });
                        }
                        browserLogReady.then((browserLog) => {
                            browser.closeBrowser(browserLog);
                        });
                    }
                });
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


    #setTestCases(testCasesPath, testCases = []) {
        return new Promise((resolve, reject) => {
            fs.lstat(testCasesPath, (err, stats) => {
                if (err) {
                    return reject(err);
                } else {
                    if (stats.isDirectory()) {
                        fs.readdir(testCasesPath, (err, items) => {
                            if (err) {
                                return reject(err);
                            } else {
                                let testCasesReady = [];
                                items.forEach(item => {
                                    let testCaseReady = this.#setTestCases(path.join(testCasesPath, item), testCases);
                                    testCasesReady.push(testCaseReady);
                                    testCaseReady.then(newTestCases => {
                                        testCases = testCases.concat(newTestCases);
                                    });
                                });
                                Promise.all(testCasesReady).then(() => {
                                    return resolve(testCases);
                                });
                            }
                        });
                    } else {
                        if (path.extname(testCasesPath) === ".mjs") {
                            let testCase = path.relative(this.#testCasesPath, testCasesPath)
                            let testCaseWoExt = path.join(path.dirname(testCase), path.basename(testCase, ".mjs"));
                            return resolve([testCaseWoExt]);
                        } else {
                            return resolve([]);
                        }
                    }
                }
            });
        });
    }


    #filterTestCases() {
        return new Promise((resolve, reject) => {
            this.#testCasesReady.then(testCases => {
                this.#testCases = testCases;
                let ans = [];
                if (this.#cfgTestCasesFilters.length === 0) {
                    ans = testCases;
                } else {
                    this.#cfgTestCasesFilters.forEach(filter => {
                        filter = path.join(path.dirname(filter), path.basename(filter, ".mjs"));
                        if (path.dirname(filter) === ".") {
                            testCases.forEach(testCase => {
                                if (path.basename(filter) === path.basename(testCase)) {
                                    ans.push(testCase);
                                }
                            });
                        } else {
                            if (testCases.includes(filter)) {
                                ans.push(filter);
                            } else {
                                let filterWoTestCasesPath = path.relative(this.#testCasesPath, filter);
                                if (testCases.includes(filterWoTestCasesPath)) {
                                    ans.push(filterWoTestCasesPath);
                                }
                            }
                        }
                    });
                }
                this.#filteredTestCases = ans;
                return resolve(ans);
            }).catch((err) => {
                return reject(err);
            });
        });
    }


    #setTestCasesHashList() {
        return new Promise((resolve, reject) => {
            fs.stat(this.#testCasesHashListPath, err => {
                if (err === null) {
                    fs.readFile(this.#testCasesHashListPath, (err, data) => {
                        if (err) {
                            return reject(err);
                        }
                        this.#testCasesHashList = JSON.parse(data);
                        return resolve(this.#testCasesHashList);
                    });
                } else if (err.code === "ENOENT") {
                    this.#testCasesHashList = {};
                    return resolve(this.#testCasesHashList);
                } else {
                    return reject(err);
                }
            });
        });
    }


    #setBrowserNumber() {
        let filteredTestCasesLength = this.#filteredTestCases.length;
        if (filteredTestCasesLength < this.#cfgBrowsersNum) {
            this.#browsersNum = filteredTestCasesLength;
        } else {
            this.#browsersNum = this.#cfgBrowsersNum;
        }
    }


    #setAnimTimeout() {
        let animTimeout = 100000;
        let rate = 0.1;
        this.#animTimeout = parseInt(animTimeout * Math.pow(1 + rate, this.#browsersNum));
    }


    #createHashes() {
        if (this.#cfgCreateHashes !== "DISABLED") {
            if (this.#cfgCreateHashes === "ALL" || (this.#cfgCreateHashes === "FAILED" && (this.#testSuiteResults.FAILED.length !==0 || this.#testSuiteResults.WARNING !== 0))) {
                let hashesPath = path.join(TestEnv.getTestSuiteResultsPath(), path.basename(this.#testCasesHashListPath));
                let rmReady = new Promise((resolve, reject) => {
                    fs.rm(hashesPath, { force: true }, err => {
                        if (err) {
                            return reject(err);
                        }
                        return resolve();
                    });
                });
                let mkdirReady = new Promise((resolve, reject) => {
                    fs.mkdir(TestEnv.getTestSuiteResultsPath(), { recursive: true }, err => {
                        if (err) {
                            return reject(err);
                        }
                        return resolve();
                    });
                });
                Promise.all([rmReady, mkdirReady]).then(() => {
                    let testCasesData = {};
                    for (const [key, value] of Object.entries(this.#testCasesResults)) {
                        if (this.#cfgCreateHashes === "FAILED" && (!this.#testSuiteResults.FAILED.includes(key) && !this.#testSuiteResults.WARNING.includes(key))) {
                            continue;
                        }
                        testCasesData[key] = {refs: [ this.#testCasesResults[key]['hash'] ] };
                    }
                    if (Object.keys(testCasesData).length !== 0) {
                        testCasesData = JSON.stringify(testCasesData, null, 4);
                        fs.writeFile(hashesPath, testCasesData, (err) => {
                            if (err) {
                                throw err;
                            }
                        });
                    }
                });
            }
        }
    }
}


module.exports = TestSuite;
