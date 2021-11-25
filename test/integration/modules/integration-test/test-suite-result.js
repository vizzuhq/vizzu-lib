const path = require("path");
const fs = require("fs");

const TestEnv = require("../../modules/integration-test/test-env.js");


class TestSuiteResult {

    #cnsl;

    #testSuiteResults;
    #testCases;
    #testCasesConfig;
    #createHashes;


    constructor(
        testSuiteResults,
        testCases,
        testCasesConfig,
        createHashes,
        cnsl
    ) {

        this.#cnsl = cnsl;

        this.#testSuiteResults = testSuiteResults;
        this.#testCases = testCases;
        this.#testCasesConfig = testCasesConfig;
        this.#createHashes = createHashes;
    }


    createTestSuiteResult() {
        this.#createNewConfig();
        this.#testSuiteResults.TIME.END = Math.round(Date.now() / 1000);
        let duration = this.#testSuiteResults.TIME.END - this.#testSuiteResults.TIME.START;
        this.#cnsl.log("\n" + "duration:".padEnd(15, " ") + duration + "s");
        this.#cnsl.log("\n" + "all tests:".padEnd(15, " ") + this.#testCases.testCases.length);
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
                this.#cnsl.log("".padEnd(this.#cnsl.getTestStatusPad() + 5, " ") + path.relative(TestEnv.getTestSuitePath(), path.join(TestEnv.getWorkspacePath(), testCase)) + " http://127.0.0.1:8080/test/integration/manual/?version=localhost&testCase=" + testCase);
            });
        } else {
            this.#cnsl.log("tests failed:".padEnd(15, " ") + this.#testSuiteResults.FAILED.length);
        }
    }


    #createNewConfig() {
        if (this.#createHashes !== "DISABLED") {
            if (this.#createHashes === "ALL" || (this.#createHashes === "FAILED" && (this.#testSuiteResults.FAILED.length !== 0 || this.#testSuiteResults.WARNING.length !== 0))) {
                let testCasesConfig = {};
                this.#testCasesConfig.suites.forEach(suite => {
                    let suitePath = "/" + path.relative(TestEnv.getWorkspacePath(), suite.suite);
                    testCasesConfig[suitePath] = { suite: suitePath, tmp: suite.config, test: {} };
                });
                for (const [key, value] of Object.entries(this.#testSuiteResults.RESULTS)) {
                    if (this.#createHashes === "FAILED" && (!this.#testSuiteResults.FAILED.includes(key) && !this.#testSuiteResults.WARNING.includes(key))) {
                        continue;
                    }
                    Object.keys(testCasesConfig).forEach(suite => {
                        if (key.startsWith(suite)) {
                            let testData = {};
                            if(this.#testSuiteResults.RESULTS[key]["animstep"]) {
                                testData["animstep"] = this.#testSuiteResults.RESULTS[key]["animstep"] + "%";
                            }
                            testData["refs"] = [this.#testSuiteResults.RESULTS[key]['hash']]
                            testCasesConfig[suite].test[path.relative(suite, key)] = testData;
                        }
                    });
                }

                for (const [key, value] of Object.entries(testCasesConfig)) {
                    if (Object.keys(testCasesConfig[key].test).length === 0) {
                        continue;
                    }
                    let conFigPath = path.join(TestEnv.getTestSuiteResultsPath(), 
                        path.relative(TestEnv.getTestSuitePath(), 
                                        path.join(TestEnv.getWorkspacePath(), 
                                                    key)), 
                        path.basename(value.tmp));
                    let rmReady = new Promise((resolve, reject) => {
                        fs.rm(conFigPath, { force: true }, err => {
                            if (err) {
                                return reject(err);
                            }
                            return resolve();
                        });
                    });
                    let mkdirReady = new Promise((resolve, reject) => {
                        fs.mkdir(path.dirname(conFigPath), { recursive: true }, err => {
                            if (err) {
                                return reject(err);
                            }
                            return resolve();
                        });
                    });
                    Promise.all([rmReady, mkdirReady]).then(() => {
                        let configData = value;
                        delete configData.tmp;
                        configData = JSON.stringify(configData, null, 4);
                        fs.writeFile(conFigPath, configData, (err) => {
                            if (err) {
                                throw err;
                            }
                        });
                    });
                }
            }
        }
    }
}


module.exports = TestSuiteResult;
