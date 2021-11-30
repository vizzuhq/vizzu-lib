const pngjs = require("pngjs");
const pixelmatch = require("pixelmatch");

const path = require("path");
const fs = require("fs");

const TestEnv = require("../../../modules/integration-test/test-env.js");
const VizzuUrl = require("../../../modules/vizzu/vizzu-url.js");


class TestCaseResult {

    #cnsl;

    #testCaseObj;
    #testData;

    #browserChrome;
    #vizzuUrl;

    #runTestCaseRef;


    constructor(testCaseObj, testData, browserChrome, vizzuUrl, runTestCaseRef) {

        this.#cnsl = testCaseObj.cnsl;
        
        this.#testCaseObj = testCaseObj;
        this.#testData = testData;
        this.#browserChrome = browserChrome;
        this.#vizzuUrl = vizzuUrl;

        this.#runTestCaseRef = runTestCaseRef;
    }


    createTestCaseResult() {
        return new Promise((resolve, reject) => {
            let deleteTestCaseResultReady = new Promise(resolve => {resolve()});
            if (this.#testCaseObj.createImages !== "DISABLED") {
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
        this.#testCaseObj.testSuiteResults.PASSED.push(this.#testCaseObj.testCase);
        this.#cnsl.log(("[ " + this.#testData.result.padEnd(this.#cnsl.getTestStatusPad(), " ") + " ] ").success + "[ " + String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(this.#cnsl.getTestNumberPad(), " ") + " ] " + path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), this.#testCaseObj.testCase)));
        if (this.#testCaseObj.createImages === "ALL") {
            this.#createImage(this.#testData, '-1new');
        }
    }


    #createTestCaseResultWarning() {
        this.#testCaseObj.testSuiteResults.WARNING.push(this.#testCaseObj.testCase);
        this.#cnsl.log(("[ " + this.#testData.result.padEnd(this.#cnsl.getTestStatusPad(), " ") + " ] " + "[ " + String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(this.#cnsl.getTestNumberPad(), " ") + " ] " + "[ " + this.#testData.description + " ] ").warn + path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), this.#testCaseObj.testCase)));
        if (this.#testCaseObj.createImages !== "DISABLED") {
            this.#createImage(this.#testData, '-1new');
        }
    }


    #createTestCaseResultFailed() {
        return new Promise((resolve, reject) => {
            this.#testCaseObj.testSuiteResults.FAILED.push(this.#testCaseObj.testCase);
            if (this.#testCaseObj.createImages !== "DISABLED") {
                this.#createImage(this.#testData, '-1new');
            }
            if (this.#testCaseObj.createImages !== "DISABLED" && !this.#vizzuUrl.includes(VizzuUrl.getRemoteStableBucket())) {
                this.#runTestCaseRef(this.#testCaseObj, this.#browserChrome).then(testDataRef => {
                    this.#createImage(testDataRef, '-2ref');
                    this.#createDifImage(this.#testData, testDataRef);
                    this.#createTestCaseResultErrorMsg();
                    let diff = false;
                    for (let i = 0; i < this.#testData.hashes.length; i++) {
                        for (let j = 0; j < this.#testData.hashes[i].length; j++) {
                            if (this.#testData.hashes[i][j] != testDataRef.hashes[i][j]) {
                                this.#cnsl.log(''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') + '[ ' + 'step: ' + i + '. - seek: ' + this.#testData.seeks[i][j] + ' - hash: ' + this.#testData.hashes[i][j].substring(0, 7) + ' ' + '(ref: ' + testDataRef.hashes[i][j].substring(0, 7) + ')' + ' ]');
                                diff = true
                            }
                        }
                    }
                    if (!diff) {
                        this.#cnsl.log(''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') + '[ the currently counted hashes are the same, the difference is probably caused by the environment ]');
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
        this.#testCaseObj.testSuiteResults.FAILED.push(this.#testCaseObj.testCase);
        this.#createTestCaseResultErrorMsg();
    }


    #createTestCaseResultErrorMsg() {
        let errParts = this.#testData.description.split("http://127.0.0.1:" + String(this.#testCaseObj.workspaceHostServerPort)).join(path.resolve(TestEnv.getWorkspacePath())).split("\n");
        this.#cnsl.log(("[ " + this.#testData.result.padEnd(this.#cnsl.getTestStatusPad(), " ") + " ] " + "[ " + String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(this.#cnsl.getTestNumberPad(), " ") + " ] " + "[ " + errParts[0] + " ] ").error + path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), this.#testCaseObj.testCase)));
        if (errParts.length > 1) {
            errParts.slice(1).forEach(item => {
                this.#cnsl.log("".padEnd(this.#cnsl.getTestStatusPad() + 7, " ") + item);
            });
        }
    }


    #deleteTestCaseResult() {
        return new Promise((resolve, reject) => {
            let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), 
                path.relative(TestEnv.getTestSuitePath(), 
                path.join(TestEnv.getWorkspacePath(), 
                    this.#testCaseObj.testCase)));
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
            let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), 
                path.relative(TestEnv.getTestSuitePath(), 
                path.join(TestEnv.getWorkspacePath(), 
                    this.#testCaseObj.testCase)));
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
        let testCaseResultPath = path.join(TestEnv.getTestSuiteResultsPath(), 
                path.relative(TestEnv.getTestSuitePath(), 
                path.join(TestEnv.getWorkspacePath(), 
                    this.#testCaseObj.testCase)));
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
