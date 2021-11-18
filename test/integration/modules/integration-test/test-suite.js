const path = require("path");
const fs = require("fs");

const pLimitReady = import("p-limit");
const AggregateErrorReady = import("aggregate-error");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const WorkspacePath = require("../../modules/workspace/workspace-path.js");
const Chrome = require("../../modules/browser/chrome.js");
const VizzuUrl = require("../../modules/integration-test/vizzu-url.js");
const { TestCaseResult } = require("../../modules/integration-test/test-case.js");


class TestSuite {

    #animStep = "20%";
    #animTimeout;

    #cfgVizzuUrl;
    #vizzuUrlReady;
    #vizzuUrl;

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

    #cwdPath;
    #workspacePath;
    #workspaceHost;
    #workspaceHostReady;
    #workspaceHostServerPort;

    #cfgBrowserGui;
    #cfgBrowsersNum;
    #browsersNum;
    #browsersList = [];

    #cfgCreateImages;
    #cfgCreateHashes;
    #cfgResultPath;
    #cfgPadLength;
    #cnsl;


    constructor(cfgCwdPath, 
                cfgWorkspacePath, 
                cfgTestCasesPath, 
                cfgTestCasesHashListPath, 
                cfgTestCasesFilters, 
                cfgVizzuUrl, 
                cfgBrowserGui, 
                cfgBrowsersNum, 
                cfgCreateImages, 
                cfgCreateHashes, 
                cfgResultPath, 
                cfgPadLength, 
                cnsl) {

        this.#cwdPath = cfgCwdPath;
        this.#workspacePath = cfgWorkspacePath;
        this.#cnsl = cnsl;

        this.#cfgTestCasesPath = cfgTestCasesPath;
        this.#cfgTestCasesHashListPath = cfgTestCasesHashListPath;
        this.#cfgTestCasesFilters = cfgTestCasesFilters;
        this.#cfgVizzuUrl = cfgVizzuUrl;
        this.#cfgBrowserGui = cfgBrowserGui;
        this.#cfgBrowsersNum = cfgBrowsersNum;
        this.#cfgCreateImages = cfgCreateImages;
        this.#cfgCreateHashes = cfgCreateHashes;
        this.#cfgResultPath = cfgResultPath;
        this.#cfgPadLength = cfgPadLength;

        this.#cnsl.log("[ " + "LOG TIME".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + this.#cnsl.getTimeStamp() + " ]");
        this.#testCasesHashListPath = WorkspacePath.resolvePath(this.#cfgTestCasesHashListPath, this.#workspacePath, this.#cwdPath);
        this.#cnsl.log("[ " + "T.HASHES".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + this.#testCasesHashListPath + " ]");
        this.#testCasesHashListReady = this.#setTestCasesHashList();
        this.#testCasesPath = WorkspacePath.resolvePath(this.#cfgTestCasesPath, this.#workspacePath, this.#cwdPath);
        this.#cnsl.log("[ " + "T.CASES".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + this.#testCasesPath + " ]");
        this.#testCasesReady = this.#setTestCases(this.#testCasesPath);
        this.#filteredTestCasesReady = this.#filterTestCases();
        Promise.all([this.#filteredTestCasesReady, this.#testCasesReady]).then(([filteredTestCases, testCases]) => {
            this.#cnsl.log("[ " + "T.CASES".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + this.#filteredTestCases.length + " / " + this.#testCases.length + " ]");
        });
        this.#vizzuUrlReady = VizzuUrl.resolveVizzuUrl(this.#cfgVizzuUrl, this.#workspacePath, this.#cwdPath);
        this.#vizzuUrlReady.then(url => {
            this.#vizzuUrl = url;
            this.#cnsl.log("[ " + "V. URL".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + url + " ]");
        });
    }


    static deleteTestSuiteReport(reportPath) {
        fs.rm(reportPath, { recursive: true, force: true }, err => {
            if (err) {
                throw err;
            }
        });
    }


    runTestSuite() {
        this.#filteredTestCasesReady.then(() => {
            if (this.#filteredTestCases.length > 0) {
                this.#setBrowserNumber();
                this.#setAnimTimeout();
                this.#startTestSuite();
                Promise.all([this.#testCasesHashListReady, this.#vizzuUrlReady, this.#workspaceHostReady, pLimitReady]).then(([ testCasesHashList, vizzuUrl, workspaceHostServerPort, pLimit ]) => {
                    const limit = pLimit.default(this.#browsersNum);
                    let testCasesReady = this.#filteredTestCases.map(testCase => {
                        return limit(() => this.#runTestCase(testCase));
                    });
                    Promise.all(testCasesReady).finally(() => {
                        this.#finishTestSuite();
                    });
                });
            }
        });
    }


    #startTestSuite() {
        this.#workspaceHost = new WorkspaceHost(this.#workspacePath);
        this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
        this.#workspaceHostReady.then(serverPort => {
            this.#workspaceHostServerPort = serverPort;
            this.#cnsl.log("[ " + "W. HOST".padEnd(this.#cfgPadLength, " ") + " ]" + " " + "[ " + "http://127.0.0.1:" + String(serverPort) + " ]");
        });
        for (let i = 0; i < this.#browsersNum; i++) {
            this.#browsersList.push(new Chrome(!this.#cfgBrowserGui));
        }
    }


    #runTestCase(testCase) {
        return new Promise((resolve, reject) => {
            let browser = this.#browsersList.shift();
            this.#runTestCaseClient(testCase, this.#vizzuUrl, browser).then(testData => {
                this.#testCasesResults[testCase] = testData;
                let testCaseResult = new TestCaseResult(testCase,
                                                        testData,
                                                        this,
                                                        this.#testSuiteResults,
                                                        browser,
                                                        this.#vizzuUrl,
                                                        this.#workspacePath,
                                                        this.#workspaceHostServerPort,
                                                        this.#cfgCreateImages,
                                                        this.#cfgResultPath,
                                                        String(this.#filteredTestCases.length).length,
                                                        this.#cfgPadLength, 
                                                        this.#cnsl);
                testCaseResult.createTestCaseResult().then((result) => {
                    this.#browsersList.push(browser);
                    resolve();
                });
            });
        });
    }


    runTestCaseRef(testCase, browser) {
        return new Promise((resolve, reject) => {
            let vizzuUrl = VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuMinJs();
            this.#runTestCaseClient(testCase, vizzuUrl, browser).then(testDataRef => {
                resolve(testDataRef);
            }).catch(err => {
                reject(err);
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
                vizzuUrl = "/" + path.relative(this.#workspacePath, vizzuUrl);
            }
            browser.getUrl("http://127.0.0.1:" + String(this.#workspaceHostServerPort)
                + "/test/integration/modules/integration-test-client/index.html"
                + "?testCasesPath=" + path.relative(this.#workspacePath, this.#testCasesPath)
                + "&testCase=" + testCase
                + "&vizzuUrl=" + vizzuUrl
                + "&animstep=" + animstep
                + "&refHash=" + refHash.toString()
                + "&createImages=" + this.#cfgCreateImages)
            .then(() => {
                browser.waitUntilTitleIs("Finished", this.#animTimeout).then(() => {
                    browser.executeScript("return testData").then(testData => {
                        resolve(testData);
                    })
                }).catch(err => {
                    let errMsg = err.toString();
                    if (!errMsg.includes("TimeoutError: Waiting for title to be \"Finished\"")) {
                        throw err;
                    }
                    resolve({ result: "ERROR", description: "Timeout" });
                });
            });
        });
    }


    #createTestSuiteResults() {
        let duration = this.#testSuiteResults.TIME.END - this.#testSuiteResults.TIME.START;
        this.#cnsl.log("\n" + "duration:".padEnd(15, " ") + duration + "s");
        this.#cnsl.log("\n" + "all tests:".padEnd(15, " ") + this.#testCases.length);
        const sum = this.#testSuiteResults.PASSED.length + this.#testSuiteResults.WARNING.length + this.#testSuiteResults.FAILED.length;
        this.#cnsl.log("tests run:".padEnd(15, " ") + sum);
        this.#cnsl.log(("tests passed:".padEnd(15, " ") + this.#testSuiteResults.PASSED.length).success);
        if (this.#testSuiteResults.WARNING.length != 0) {
            this.#cnsl.log(("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length).warn);
        } else {
            this.#cnsl.log("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length);
        }
        if (this.#testSuiteResults.FAILED.length != 0) {
            this.#cnsl.log(("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length).error);
            process.exitCode = 1;
            this.#testSuiteResults.FAILED.forEach(testCase => {
                this.#cnsl.log("".padEnd(this.#cfgPadLength + 5, " ") + testCase + " http://127.0.0.1:8080/test/integration/manual/?version=localhost&testCase=" + testCase);
            });
        } else {
            this.#cnsl.log("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length);
        }
    }


    #finishTestSuite() {
        AggregateErrorReady.then(AggregateError => {
            let errs = [];
            try {
                this.#testSuiteResults.TIME.END = Math.round(Date.now() / 1000);
                this.#createTestSuiteResults();
                this.#createHashes();
            } catch (err) {
                errs.push(err);
            }
            try {
                this.#browsersList.forEach((browser, index) => {
                    if (browser) {
                        let browserLog;
                        if (!cfgNoLogs) {
                            let logPath = path.dirname(this.#cnsl.getLogFile());
                            fs.mkdir(logPath, { recursive: true, force: true }, err => {
                                if (err) {
                                    throw err;
                                } else {
                                    browserLog = path.join(logPath, "chromedriver_" + index  + '_' + this.#cnsl.getTimeStamp() + ".log");
                                    browser.closeBrowser(browserLog);
                                }
                            });
                        } else {
                            browser.closeBrowser(browserLog);
                        }
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
                if (err)
                    reject(err);
                else {
                    if (stats.isDirectory()) {
                        fs.readdir(testCasesPath, (err, items) => {
                            if (err)
                                reject(err);
                            else {
                                let testCasesReady = [];
                                items.forEach(item => {
                                    let testCaseReady = this.#setTestCases(path.join(testCasesPath, item), testCases);
                                    testCasesReady.push(testCaseReady);
                                    testCaseReady.then(newTestCases => {
                                        testCases = testCases.concat(newTestCases);
                                    });
                                });
                                Promise.all(testCasesReady).then(() => {
                                    resolve(testCases);
                                });
                            }
                        });
                    } else {
                        if (path.extname(testCasesPath) === ".mjs") {
                            let testCase = path.relative(this.#testCasesPath, testCasesPath)
                            let testCaseWoExt = path.join(path.dirname(testCase), path.basename(testCase, ".mjs"));
                            resolve([testCaseWoExt]);
                        } else {
                            resolve([]);
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
                resolve(ans);
            }).catch((err) => {
                reject(err);
            });
        });
    }


    #setTestCasesHashList() {
        return new Promise((resolve, reject) => {
            fs.stat(this.#testCasesHashListPath, err => {
                if (err === null) {
                    fs.readFile(this.#testCasesHashListPath, (err, data) => {
                        if (err) {
                            reject(err);
                        }
                        this.#testCasesHashList = JSON.parse(data);
                        resolve(this.#testCasesHashList);
                    });
                } else if (err.code === "ENOENT") {
                    this.#testCasesHashList = {};
                    resolve(this.#testCasesHashList);
                } else {
                    reject(err);
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
                let hashesPath = path.join(this.#cfgResultPath, path.basename(this.#testCasesHashListPath));
                let rmReady = new Promise((resolve, reject) => {
                    fs.rm(hashesPath, { force: true }, err => {
                        if (err) {
                            reject(err);
                        }
                        resolve();
                    });
                });
                let mkdirReady = new Promise((resolve, reject) => {
                    fs.mkdir(this.#cfgResultPath, { recursive: true }, err => {
                        if (err) {
                            reject(err);
                        }
                        resolve();
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
