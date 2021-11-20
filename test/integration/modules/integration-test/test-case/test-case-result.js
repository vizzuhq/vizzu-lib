const pngjs = require("pngjs");
const pixelmatch = require("pixelmatch");

const path = require("path");
const fs = require("fs");

const TestEnv = require("../../../modules/integration-test/test-env.js");
const VizzuUrl = require("../../../modules/integration-test/vizzu-url.js");


class TestCaseResult {

    #testConsole;

    #testCase
    #testData;

    #testSuite;
    #testSuiteResults;

    #browser;
    #vizzuUrl;
    #workspaceHostServerPort;

    #cfgCreateImages;
    #cfgNumPadLength;


    constructor(testConsole,
                testCase,
                testData,
                testSuite,
                testSuiteResults,
                browser, 
                vizzuUrl,
                workspaceHostServerPort,
                cfgCreateImages,
                cfgNumPadLength) {
        
        this.#testConsole = testConsole;
        
        this.#testCase = testCase;
        this.#testData = testData;
        this.#testSuite = testSuite;
        this.#testSuiteResults = testSuiteResults;
        this.#browser = browser;
        this.#vizzuUrl = vizzuUrl;
        this.#workspaceHostServerPort = workspaceHostServerPort;
        this.#cfgCreateImages = cfgCreateImages;
        this.#cfgNumPadLength = cfgNumPadLength;
    }


    createTestCaseResult() {
        return new Promise((resolve, reject) => {
            let deleteTestCaseResultReady = new Promise(resolve => {resolve()});
            if (this.#cfgCreateImages !== "DISABLED") {
                deleteTestCaseResultReady = this.#deleteTestCaseResult();
            }
            deleteTestCaseResultReady.then(() => {
                if (this.#testData.result == "PASSED") {
                    return resolve(this.#createTestCaseResultPassed());
                } else if(this.#testData.result == "WARNING") {
                    return resolve(this.#createTestCaseResultWarning());
                } else if(this.#testData.result == "FAILED") {
                    return resolve(this.#createTestCaseResultFailed());
                } else {
                    return resolve(this.#createTestCaseResultError());
                }
            });
        });
    }


    #createTestCaseResultPassed() {
        this.#testSuiteResults.PASSED.push(this.#testCase);
        this.#testConsole.log(("[ " + this.#testData.result.padEnd(this.#testConsole.getTestStatusPad(), " ") + " ] ").success + "[ " + String(++this.#testSuiteResults.FINISHED).padEnd(this.#cfgNumPadLength, " ") + " ] " + this.#testCase);
        if (this.#cfgCreateImages === "ALL") {
            this.#createImage(this.#testData, '-1new');
        }
    }


    #createTestCaseResultWarning() {
        this.#testSuiteResults.WARNING.push(this.#testCase);
        this.#testConsole.log(("[ " + this.#testData.result.padEnd(this.#testConsole.getTestStatusPad(), " ") + " ] " + "[ " + String(++this.#testSuiteResults.FINISHED).padEnd(this.#cfgNumPadLength, " ") + " ] " + "[ " + this.#testData.description + " ] ").warn + this.#testCase);
        if (this.#cfgCreateImages !== "DISABLED") {
            this.#createImage(this.#testData, '-1new');
        }
    }


    #createTestCaseResultFailed() {
        return new Promise((resolve, reject) => {
            this.#testSuiteResults.FAILED.push(this.#testCase);
            if (this.#cfgCreateImages !== "DISABLED") {
                this.#createImage(this.#testData, '-1new');
            }
            if (this.#cfgCreateImages !== "DISABLED" && !this.#vizzuUrl.includes(VizzuUrl.getRemoteStableBucket())) {
                this.#testSuite.runTestCaseRef(this.#testCase, this.#browser).then(testDataRef => {
                    this.#createImage(testDataRef, '-2ref');
                    this.#createDifImage(this.#testData, testDataRef);
                    this.#createTestCaseResultErrorMsg();
                    let diff = false;
                    for (let i = 0; i < this.#testData.hashes.length; i++) {
                        for (let j = 0; j < this.#testData.hashes[i].length; j++) {
                            if (this.#testData.hashes[i][j] != testDataRef.hashes[i][j]) {
                                this.#testConsole.log(''.padEnd(this.#testConsole.getTestStatusPad() + 5, ' ') + '[ ' + 'step: ' + i + '. - seek: ' + this.#testData.seeks[i][j] + ' - hash: ' + this.#testData.hashes[i][j].substring(0, 7) + ' ' + '(ref: ' + testDataRef.hashes[i][j].substring(0, 7) + ')' + ' ]');
                                diff = true
                            }
                        }
                    }
                    if (!diff) {
                        this.#testConsole.log(''.padEnd(this.#testConsole.getTestStatusPad() + 5, ' ') + '[ the currently counted hashes are the same, the difference is probably caused by the environment ]');
                    }
                    return resolve();
                });
            } else {
                this.#createTestCaseResultErrorMsg();
                return resolve();
            }
        });        
    }


    #createTestCaseResultError() {
        this.#testSuiteResults.FAILED.push(this.#testCase);
        this.#createTestCaseResultErrorMsg();
    }


    #createTestCaseResultErrorMsg() {
        let errParts = this.#testData.description.split("http://127.0.0.1:" + String(this.#workspaceHostServerPort)).join(path.resolve(TestEnv.getWorkspacePath())).split("\n");
        this.#testConsole.log(("[ " + this.#testData.result.padEnd(this.#testConsole.getTestStatusPad(), " ") + " ] " + "[ " + String(++this.#testSuiteResults.FINISHED).padEnd(this.#cfgNumPadLength, " ") + " ] " + "[ " + errParts[0] + " ] ").error + this.#testCase);
        if (errParts.length > 1) {
            errParts.slice(1).forEach(item => {
                this.#testConsole.log("".padEnd(this.#testConsole.getTestStatusPad() + 7, " ") + item);
            });
        }
    }


    #deleteTestCaseResult() {
        return new Promise((resolve, reject) => {
            let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), this.#testCase);
            fs.rm(testCaseResultPath, { recursive: true, force: true }, err => {
                if (err) {
                    return reject(err);
                }
                return resolve();
            });
        });
    }


    #createImage(data, fileAdd) {
        return new Promise((resolve, reject) => {
            let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), this.#testCase);
            fs.mkdir(testCaseResultPath, { recursive: true, force: true }, err => {
                if (err) {
                    return reject(err);
                }
                for (let i = 0; i < data.seeks.length; i++) {
                    for (let j = 0; j < data.seeks[i].length; j++) {
                        let seek = (data.seeks[i][j].replace('%', '')).split('.');
                        if (seek.length == 1) {
                            seek.push('0');
                        }
                        fs.writeFile(testCaseResultPath + '/' + path.basename(testCaseResultPath) + '_' + i.toString().padStart(3, '0') + '_' + seek[0].padStart(3, '0') + '.' + seek[1].padEnd(3, '0') + '%' + fileAdd + '.png', data.images[i][j].substring(22), 'base64', err => {
                            if (err) {
                                return reject(err);
                            }
                            return resolve()
                        });
                    }
                }
            });
        });
    }


    #createDifImage(testData, testDataRef) {
        let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), this.#testCase);
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
}


module.exports = TestCaseResult;
