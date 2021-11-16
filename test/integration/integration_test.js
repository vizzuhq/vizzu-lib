const path = require("path");
const fs = require("fs");
const yargs = require("yargs");
const fetch = require("node-fetch");
const strip = require("strip-color");
const pngjs = require("pngjs");
const pixelmatch = require("pixelmatch");
const colors = require("colors");
colors.setTheme({
    warn: "yellow",
    error: "red",
    success: "green"
});

const pLimitReady = import("p-limit");
const AggregateErrorReady = import("aggregate-error");

const WorkspaceHost = require("./modules/workspace/workspace-host.js");
const Chrome = require("./modules/browser/chrome.js");


const remoteBucket = "https://vizzu-lib-main-sha.storage.googleapis.com";
const remoteStableBucket = "https://vizzu-lib-main.storage.googleapis.com";
const remoteCdn = "https://cdn.jsdelivr.net/npm/vizzu";
const reportPath = path.join(__dirname, "test_report");
const resultPath = path.join(reportPath, "results");
const logPath = path.join(reportPath, "logs");
const padLength = 8;


class TestSuite {

    #animStep = "20%";
    #animTimeout;

    #cfgVizzuUrl;
    #vizzuUrlReady;
    #vizzuUrl;
    #vizzuJs = "/vizzu.js";
    #vizzuMinJs = "/vizzu.min.js";

    #testSuiteResults = { PASSED: [], WARNING: [], FAILED: [], TIME: { START: Math.round(Date.now() / 1000), END: 0 } };

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
    
    #testCasesFinished = 0;
    #testCasesResults = {};

    #workspacePath = path.join(__dirname, "../..");
    #workspaceHost;
    #workspaceHostReady;
    #workspaceHostServerPort;

    #cfgBrowsersNum;
    #browsersNum;
    #browsersList = [];

    #cfgCreateImages;
    #cfgCreateHashes;


    constructor(cfgTestCasesPath, cfgTestCasesHashListPath, cfgTestCasesFilters, cfgVizzuUrl, cfgBrowsersNum, cfgCreateImages, cfgCreateHashes) {
        this.#cfgTestCasesPath = cfgTestCasesPath;
        this.#cfgTestCasesHashListPath = cfgTestCasesHashListPath;
        this.#cfgTestCasesFilters = cfgTestCasesFilters;
        this.#cfgVizzuUrl = cfgVizzuUrl;
        this.#cfgBrowsersNum = cfgBrowsersNum;
        this.#cfgCreateImages = cfgCreateImages;
        this.#cfgCreateHashes = cfgCreateHashes;

        log("[ " + "LOG TIME".padEnd(padLength, " ") + " ]" + " " + "[ " + timestamp + " ]");
        this.#testCasesHashListPath = this.#resolvePath(this.#cfgTestCasesHashListPath);
        log("[ " + "T.HASHES".padEnd(padLength, " ") + " ]" + " " + "[ " + this.#testCasesHashListPath + " ]");
        this.#testCasesHashListReady = this.#setTestCasesHashList();
        this.#testCasesPath = this.#resolvePath(this.#cfgTestCasesPath);
        log("[ " + "T.CASES".padEnd(padLength, " ") + " ]" + " " + "[ " + this.#testCasesPath + " ]");
        this.#testCasesReady = this.#setTestCases(this.#testCasesPath);
        this.#filteredTestCasesReady = this.#filterTestCases();
        Promise.all([this.#filteredTestCasesReady, this.#testCasesReady]).then(([filteredTestCases, testCases]) => {
            log("[ " + "T.CASES".padEnd(padLength, " ") + " ]" + " " + "[ " + this.#filteredTestCases.length + " / " + this.#testCases.length + " ]");
        });
        this.#vizzuUrlReady = this.#setVizzuUrl();
        this.#vizzuUrlReady.then(url => {
            log("[ " + "V. URL".padEnd(padLength, " ") + " ]" + " " + "[ " + url + " ]");
        });
    }


    static deleteTestSuiteReport() {
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
            log("[ " + "W. HOST".padEnd(padLength, " ") + " ]" + " " + "[ " + "http://127.0.0.1:" + String(serverPort) + " ]");
        });
        for (let i = 0; i < this.#browsersNum; i++) {
            this.#browsersList.push(new Chrome(!argv.gui));
        }
    }


    #runTestCase(testCase) {
        return new Promise((resolve, reject) => {
            let browser = this.#browsersList.shift();
            this.#runTestCaseClient(testCase, this.#vizzuUrl, browser).then(testData => {
                this.#testCasesResults[testCase] = testData;
                this.#createTestCaseResult(testCase, testData, browser).then(() => {
                    this.#browsersList.push(browser);
                    resolve();
                });
            });
        });
    }


    #runTestCaseRef(testCase, browser) {
        return new Promise((resolve, reject) => {
            let vizzuUrl = remoteStableBucket + '/lib' + this.#vizzuMinJs;
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
                });
            });
        });
    }


    #createTestCaseResult(testCase, testData, browser) {
        return new Promise((resolve, reject) => {
            let testCasesFinished = ++this.#testCasesFinished;
            if (testData.result == "PASSED") {
                log(("[ " + testData.result.padEnd(padLength, " ") + " ] ").success + "[ " + String(testCasesFinished).padEnd(String(this.#filteredTestCases.length).length, " ") + " ] " + testCase);
                this.#testSuiteResults.PASSED.push(testCase);
            } else if (testData.result == "WARNING") {
                log(("[ " + testData.result.padEnd(padLength, " ") + " ] " + "[ " + String(testCasesFinished).padEnd(String(this.#filteredTestCases.length).length, " ") + " ] " + "[ " + testData.description + " ] ").warn + testCase);
                this.#testSuiteResults.WARNING.push(testCase);
            } else {
                let errParts = testData.description.split("http://127.0.0.1:" + String(this.#workspaceHostServerPort)).join(path.resolve(this.#workspacePath)).split("\n");
                log(("[ " + testData.result.padEnd(padLength, " ") + " ] " + "[ " + String(testCasesFinished).padEnd(String(this.#filteredTestCases.length).length, " ") + " ] " + "[ " + errParts[0] + " ] ").error + testCase);
                if (errParts.length > 1) {
                    errParts.slice(1).forEach(item => {
                        log("".padEnd(padLength + 7, " ") + item);
                    });
                }
                this.#testSuiteResults.FAILED.push(testCase);
            }
            if (this.#cfgCreateImages !== "DISABLED") {
                if (this.#cfgCreateImages === "ALL" || (this.#cfgCreateImages === "FAILED" && (testData.result === 'FAILED' || testData.result === 'WARNING'))) {
                    let deleteTestCaseResultReady = this.#deleteTestCaseResult(testCase);
                    deleteTestCaseResultReady.then(() => {
                        this.#createImage(testCase, testData, '-1new');
                    })
                    if (!this.#vizzuUrl.includes(remoteStableBucket)) {
                        if (testData.result === 'FAILED') {
                            this.#runTestCaseRef(testCase, browser).then(testDataRef => {
                                deleteTestCaseResultReady.then(() => {
                                    this.#createImage(testCase, testDataRef, '-2ref');
                                    this.#createDifImage(testCase, testData, testDataRef);
                                })
                                let diff = false;
                                for (let i = 0; i < testData.hashes.length; i++) {
                                    for (let j = 0; j < testData.hashes[i].length; j++) {
                                        if (testData.hashes[i][j] != testDataRef.hashes[i][j]) {
                                            log(''.padEnd(padLength + 5, ' ') + '[ ' + 'step: ' + i + '. - seek: ' + testData.seeks[i][j] + ' - hash: ' + testData.hashes[i][j].substring(0, 7) + ' ' + '(ref: ' + testDataRef.hashes[i][j].substring(0, 7) + ')' + ' ]');
                                            diff = true
                                        }
                                    }
                                }
                                if (!diff) {
                                    log(''.padEnd(padLength + 5, ' ') + '[ the currently counted hashes are the same, the difference is probably caused by the environment ]');
                                }
                                resolve();
                            })
                        } else {
                            resolve();
                        }
                    } else {
                        resolve();
                    }
                } else {
                    resolve();
                }
            } else {
                resolve();
            }
        });
    }


    #deleteTestCaseResult(testCase) {
        return new Promise((resolve, reject) => {
            let testCaseResultPath = path.join(resultPath, testCase);
            fs.rm(testCaseResultPath, { recursive: true, force: true }, err => {
                if (err) {
                    reject(err);
                }
                fs.mkdir(testCaseResultPath, { recursive: true, force: true }, err => {
                    if (err) {
                        reject(err);
                    }
                    resolve();
                });
            });
        });
    }


    #createTestSuiteResults() {
        let duration = this.#testSuiteResults.TIME.END - this.#testSuiteResults.TIME.START;
        log("\n" + "duration:".padEnd(15, " ") + duration + "s");
        log("\n" + "all tests:".padEnd(15, " ") + this.#testCases.length);
        const sum = this.#testSuiteResults.PASSED.length + this.#testSuiteResults.WARNING.length + this.#testSuiteResults.FAILED.length;
        log("tests run:".padEnd(15, " ") + sum);
        log(("tests passed:".padEnd(15, " ") + this.#testSuiteResults.PASSED.length).success);
        if (this.#testSuiteResults.WARNING.length != 0) {
            log(("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length).warn);
        } else {
            log("tests warning:".padEnd(15, " ") + this.#testSuiteResults.WARNING.length);
        }
        if (this.#testSuiteResults.FAILED.length != 0) {
            log(("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length).error);
            process.exitCode = 1;
            this.#testSuiteResults.FAILED.forEach(testCase => {
                log("".padEnd(padLength + 5, " ") + testCase + " http://127.0.0.1:8080/test/integration/manual/?version=localhost&testCase=" + testCase);
            });
        } else {
            log("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length);
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
                            browserLog = path.join(logPath, "chromedriver_" + index  + '_' + timestamp + ".log");
                            fileConsoleReady.then(() => {
                                browser.closeBrowser(browserLog);
                            })
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


    #setVizzuUrl() {
        return new Promise((resolve, reject) => {
            let url = this.#cfgVizzuUrl;
            let vizzuTypeForced = this.#isVizzuUrlForced(url);
            url = this.#purifyVizzuUrl(url);
            url = this.#completeVizzuUrl(url, vizzuTypeForced);
            if (url.startsWith("https://")) {
                this.#setRemoteVizzuUrl(url).then(existingUrl => {
                    this.#vizzuUrl = existingUrl;
                    resolve(existingUrl);
                }).catch((err) => {
                    reject(err);
                });
            } else {
                this.#setLocalVizzuUrl(this.#resolvePath(url)).then(vizzuPath => {
                    this.#vizzuUrl = vizzuPath;
                    resolve(vizzuPath);
                }).catch((err) => {
                    reject(err);
                });
            }
        });
    }


    #createImage(testCase, data, fileAdd) {
        return new Promise((resolve, reject) => {
            let testCaseResultPath = path.join(resultPath, testCase);
            for (let i = 0; i < data.seeks.length; i++) {
                for (let j = 0; j < data.seeks[i].length; j++) {
                    let seek = (data.seeks[i][j].replace('%', '')).split('.');
                    if (seek.length == 1) {
                        seek.push('0');
                    }
                    fs.writeFile(testCaseResultPath + '/' + path.basename(testCaseResultPath) + '_' + i.toString().padStart(3, '0') + '_' + seek[0].padStart(3, '0') + '.' + seek[1].padEnd(3, '0') + '%' + fileAdd + '.png', data.images[i][j].substring(22), 'base64', err => {
                        if (err) {
                            reject(err);
                        }
                        resolve()
                    });
                }
            }
        });
    }


    #createDifImage(testCase, testData, testDataRef) {
        let testCaseResultPath = path.join(resultPath, testCase);
        for (let i = 0; i < testData.seeks.length; i++) {
            for (let j = 0; j < testData.seeks[i].length; j++) {
                let seek = (testData.seeks[i][j].replace('%', '')).split('.');
                if (seek.length == 1) {
                    seek.push('0');
                }
                const img1 = pngjs.PNG.sync.read(Buffer.from(testData.images[i][j].substring(22), "base64"));
                const img2 = pngjs.PNG.sync.read(Buffer.from(testDataRef.images[i][j].substring(22), "base64"));
                const { width, height } = img1;
                const diff = new pngjs.PNG({ width, height });
                const difference = pixelmatch(img1.data, img2.data, diff.data, width, height, { threshold: 0 });
                if (difference) {
                    fs.writeFile(testCaseResultPath + '/' + path.basename(testCaseResultPath) + '_' + i.toString().padStart(3, '0') + '_' + seek[0].padStart(3, '0') + '.' + seek[1].padEnd(3, '0') + '%' + '-3diff' + '.png', pngjs.PNG.sync.write(diff), err => {
                        if (err) {
                            throw err;
                        }
                    });
                }
            }
        }
    }


    #createHashes() {
        if (this.#cfgCreateHashes !== "DISABLED") {
            if (this.#cfgCreateHashes === "ALL" || (this.#cfgCreateHashes === "FAILED" && (this.#testSuiteResults.FAILED.length !==0 || this.#testSuiteResults.WARNING !== 0))) {
                let hashesPath = path.join(resultPath, path.basename(this.#testCasesHashListPath));
                let rmReady = new Promise((resolve, reject) => {
                    fs.rm(hashesPath, { force: true }, err => {
                        if (err) {
                            reject(err);
                        }
                        resolve();
                    });
                });
                let mkdirReady = new Promise((resolve, reject) => {
                    fs.mkdir(resultPath, { recursive: true }, err => {
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


    #resolvePath(p) {
        if (path.isAbsolute(p)) {
            return path.resolve(path.join(this.#workspacePath, p));
        } else {
            return path.resolve(path.join(__dirname, p));
        }
    }


    #isUrlExist(url) {
        return new Promise((resolve, reject) => {
            fetch(url, {
                method: "HEAD"
            }).then(response => {
                if (response.status == 200) {
                    resolve(url);
                } else {
                    reject(response.status);
                }
            }).catch((err) => {
                reject(err);
            });
        });            
    }

    
    #isVizzuUrlForced(url) {
        if (url.endsWith(this.#vizzuMinJs)) {
            return this.#vizzuMinJs;
        } else if (url.endsWith(this.#vizzuJs)) {
            return this.#vizzuJs
        } else {
            return
        }
    }


    #purifyVizzuUrl(url) {
        if (url.endsWith(this.#vizzuMinJs)) {
            return url.substring(0, url.length - this.#vizzuMinJs.length);
        } else if (url.endsWith(this.#vizzuJs)) {
            return url.substring(0, url.length - this.#vizzuJs.length);
        } else if (url.endsWith("/")) {
            return url.substring(0, url.length - 1);
        } else {
            return url;
        }
    }


    #completeVizzuUrl(url, vizzuTypeForced) {
        if (url.toLowerCase() === "head") {
            if (!vizzuTypeForced) {
                vizzuTypeForced = this.#vizzuMinJs;
            }
            return remoteStableBucket + "/lib" + vizzuTypeForced;
        } else if (/^[A-Za-z0-9]+$/.test(url) && url.length === 7) {
            if (!vizzuTypeForced) {
                vizzuTypeForced = this.#vizzuMinJs;
            }
            return remoteBucket + "/lib-" + url + vizzuTypeForced;
        } else if (/^(\d+\.)?(\d+\.)?(\*|\d+)$/.test(url)) {
            if (vizzuTypeForced === this.#vizzuJs) {
                throw new Error("select Vizzu with a version number can be used with vizzu.js only");
            }
            return remoteCdn + "@" + url + "/dist" + this.#vizzuMinJs;
        } else {
            if (!vizzuTypeForced) {
                vizzuTypeForced = this.#vizzuJs;
            }
            return url + vizzuTypeForced;
        }
    }


    #setRemoteVizzuUrl(url) {
        return new Promise((resolve, reject) => {
            this.#isUrlExist(url).then(existingUrl => {
                resolve(existingUrl);
            }).catch((err) => {
                reject(err);
            });
        });
    }


    #setLocalVizzuUrl(vizzuPath) {
        return new Promise((resolve, reject) => {
            fs.stat(vizzuPath, err => {
                if (err === null) {
                    resolve(vizzuPath)
                } else {
                    reject(err);
                }
            });
        });
    }
}


const date = new Date();
const timestamp = date.getFullYear() + ("0" + (date.getMonth() + 1)).slice(-2) + ("0" + date.getDate()).slice(-2) + "_" + date.getHours() + date.getMinutes() + date.getSeconds();
const console = require("console");
const { Console } = console;
let cfgNoLogs = true;
let fileConsoleReady;
const log = (message) => {
    console.log(message);
    if (!cfgNoLogs) {
        if (fileConsoleReady === undefined) {
            fileConsoleReady = new Promise((resolve, reject) => {
                fs.mkdir(logPath, { recursive: true }, err => {
                    if (err) {
                        reject(err);
                    }
                    const out = fs.createWriteStream(path.join(logPath, "integration_test_" + timestamp + ".log"));
                    fileConsole = new console.Console(out, out);
                    resolve(fileConsole);
                });
            });
        }
        fileConsoleReady.then(fileConsole => {
            fileConsole.log(strip(message));
        });
    }
};


try {
    var argv = yargs

        .usage("Usage: $0 [tests] [options]" + 
                    "\n\nThe objective of the integration test is to E2E test the Vizzu library." + 
                    "\nThe test cases are animations, each animation consists of an animate function call promise chain." + 
                    "\nThe test animations run in Chrome using ChromeDriver and Selenium Webdriver." + 
                    "\nA test case seeks through each animate function calls with a predefined animation step." +
                    "\nA hash is calculated on every created canvas image data for each animation step." + 
                    "\nA test validates a hash calculated from the created hash list during each test case.")

        .help("h")
        .alias("h", "help")

        .version("0.1.0")
        .alias("v", "version")

        .string("ref")
        .nargs("ref", 1)
        .describe("ref", 
                    "Change reference hash list file path of test cases" + 
                    "\n(relative or absolute path where the repo folder is the root)" + 
                    "\n")
        .default("ref", 
                    "/test/integration/test_cases/ref_list.json")

        .string("cases")
        .nargs("cases", 1)
        .describe("cases", 
                    "Change folder path of test cases" + 
                    "\n(relative or absolute path where the repo folder is the root)" + 
                    "\n")
        .default("cases", 
                    "/test/integration/test_cases")

        .choices("images", 
                    ["ALL", "FAILED", "DISABLED"])
        .describe("images", 
                    "Change report images saving behavior" + 
                    "\n- \"ALL\": Create images for every test" + 
                    "\n- \"FAILED\": Create images for failed tests only" + 
                    "\n- \"DISABLED\": Do not create images")
        .default("images", "FAILED")

        .choices("hashes", 
                    ["ALL", "FAILED", "DISABLED"])
        .describe("hashes", 
                    "Change report hashes saving behavior" + 
                    "\n- \"ALL\": Write hashes into the report file for every test" + 
                    "\n- \"FAILED\": Write hashes into the report file for failed tests only" + 
                    "\n- \"DISABLED\": Do not create report file")
        .default("hashes", "FAILED")

        .boolean("nologs")
        .describe("nologs", 
                    "\n Do not save browser and console log into file" + 
                    "\n")
        .default("logs", false)

        .string("vizzu")
        .nargs("vizzu", 1)
        .describe("vizzu", 
                    "Change Vizzu url" + 
                    "\n(can be forced to use vizzu.js or vizzu.min.js if its given)" + 
                    "\n\n- \"head\": select the last stable Vizzu from the main branch" + 
                    "\n(default: vizzu.min.js)" + 
                    "\n\n- [sha]: select Vizzu with a short commit number" + 
                    "\n(default: vizzu.min.js)" + 
                    "\n\n- [version]: select Vizzu with a version number" + 
                    "\n(vizzu.min.js only)" + 
                    "\n\n- path: select Vizzu from the local file system" + 
                    "\n(relative or absolute path where the repo folder is the root)" +  
                    "\n(default: vizzu.js)" + 
                    "\n")
        .default("vizzu", 
                    "/example/lib/vizzu.js")

        .boolean("g")
        .alias("g", "gui")
        .describe("g", 
                    "Use browser with graphical user interface" + 
                    "\n")
        .default("g", false)

        .number("b")
        .alias("b", "browsers")
        .describe("b", 
                    "Change number of parallel browser windows" + 
                    "\n")
        .default("b", 3)


        .boolean("delete")
        .describe("delete", 
                    "Delete test report folder" + 
                    "\n")
        .default("g", false)

        .example([
                    ["$0", 
                        "Run all tests from the test cases folder"],
                    ["$0 area_carte_2_polar.mjs", 
                        "Select test case with name"],
                    ["$0 area_carte_2_polar", 
                        "Select test case with name (without extension)"],
                    ["$0 area_carte_2_polar rectangle_carte_2_polar", 
                        "Select test cases with name"],
                    ["$0 basic_animations/coordsystems/area_carte_2_polar.mjs", 
                        "Select test case with path"],
                    ["$0 ./test_cases/basic_animations/coordsystems/*", 
                        "Select test cases with glob pattern"]
                ])
        .example("$0 -g", 
                    "Run all tests and use browser with graphical user interface")
        .example([
                    ["$0 -vizzu head", 
                        "Run all tests with the latest stable Vizzu from the main branch"],
                    ["$0 -vizzu [sha]/vizzu.js", 
                        "Run all tests and select Vizzu with a short commit number" + 
                        "\nand use vizzu.js instead of the default vizzu.min.js"]
                ])

        .argv;

    
    if (!argv.delete) {
        cfgNoLogs = argv.nologs;
        let testSuite = new TestSuite(argv.cases, argv.ref, argv._, argv.vizzu, argv.browsers, argv.images, argv.hashes);
        testSuite.runTestSuite();
    } else {
        TestSuite.deleteTestSuiteReport();
    }
} catch (err) {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    log("[ " + "ERROR".padEnd(padLength, " ") + " ] " + errMsg);
}
