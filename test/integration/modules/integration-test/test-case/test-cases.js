const path = require("path");
const fs = require("fs");

const assert = require("../../../modules/console/assert.js"); 
const WorkspacePath = require("../../../modules/workspace/workspace-path.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");
const TestCasesConfig = require("../../../modules/integration-test/test-case/test-cases-config.js");


class TestCases {

    static getTestCases(testCasesConfigReady, filters) {
        return new Promise((resolve, reject) => {
            testCasesConfigReady.then(configs => {
                try {
                    assert(TestCasesConfig.isTestCasesConfig(configs), "test cases config schema validation failed");
                    let testCasesReadyList = [];
                    let filteredTestCasesReadyList = [];
                    configs.suites.forEach(suite => {
                        let testCasesReady = TestCases.collectTestCases(suite.suite);
                        testCasesReadyList.push(testCasesReady);
                        testCasesReady.then(testCases => {
                            let filteredTestCasesReady = TestCases.filterTestCases(testCases, suite.suite, filters);
                            filteredTestCasesReadyList.push(filteredTestCasesReady);
                        }).catch(err => {
                            return reject(err);
                        });
                    });
                    Promise.all(testCasesReadyList).then(testCasesList => {
                        testCasesList = testCasesList.flat(1);
                        Promise.all(filteredTestCasesReadyList).then(filteredTestCasesList => {
                            filteredTestCasesList = filteredTestCasesList.flat(1);
                            testCasesList.sort((a, b) => (a.testName > b.testName) ? 1 : -1);
                            filteredTestCasesList.sort((a, b) => (a.testName > b.testName) ? 1 : -1);
                            return resolve({testCases: testCasesList, filteredTestCases: filteredTestCasesList});
                        });
                    }).catch(err => {
                        return reject(err);
                    });
                } catch(err) {
                    return reject(err);
                }
            });
        });
    }


    static collectTestCases(p, testCases = []) {
        return new Promise((resolve, reject) => {
            fs.lstat(p, (err, stats) => {
                if (err) {
                    return reject(err);
                } else {
                    if (stats.isDirectory()) {
                        fs.readdir(p, (err, items) => {
                            if (err) {
                                return reject(err);
                            } else {
                                let testCasesReady = [];
                                items.forEach(item => {
                                    let testCaseReady = TestCases.collectTestCases(path.join(p, item), testCases);
                                    testCasesReady.push(testCaseReady);
                                    testCaseReady.then(newTestCases => {
                                        testCases = testCases.concat(newTestCases);
                                    }).catch(err => {
                                        return reject(err);
                                    });
                                });
                                Promise.all(testCasesReady).then(() => {
                                    return resolve(testCases);
                                }).catch(err => {
                                    return reject(err);
                                });
                            }
                        });
                    } else {
                        if (path.extname(p) === ".mjs") {
                            TestCases.preprocessTestCases(p).then(testCases => {
                                return resolve(testCases);
                            }).catch(err => {
                                return reject(err);
                            });
                        } else {
                            return resolve([]);
                        }
                    }
                }
            });
        });
    }


    static filterTestCases(testCases, suitePath, filters=[]) {
        return new Promise((resolve, reject) => {
            let filteredTestCases = [];
            if (filters.length === 0) {
                filteredTestCases = testCases;
            } else {
                let testKeys = {};
                testCases.forEach(testCase => {
                    if (testCase.testName !== testCase.testFile) {
                        if (testKeys[testCase.testFile] !== undefined) {
                            testKeys[testCase.testFile].push(testCase);
                        } else {
                            testKeys[testCase.testFile] = [testCase];
                        }
                    }
                    testKeys[testCase.testName] = [testCase];
                });
                filters.forEach(filter => {
                    let parsedFilter = path.parse(filter);
                    if (parsedFilter.ext === ".mjs") {
                        filter = path.join(parsedFilter.dir, parsedFilter.name);
                    }
                    if (path.dirname(filter) === ".") {
                        testCases.forEach(testCase => {
                            if (path.basename(filter) === path.basename(testCase.testName)
                                    || path.basename(filter) === path.basename(testCase.testFile)) {
                                filteredTestCases.push(testCase);
                            }
                        });
                    } else {
                        if (testKeys[filter]) {
                            filteredTestCases = filteredTestCases.concat(testKeys[filter]);
                        } else {
                            let filterPathInSuite = "/" + path.join(
                                path.relative(
                                    TestEnv.getWorkspacePath(), 
                                    suitePath), 
                                filter);
                            let filterRelative = "/" + path.relative(
                                TestEnv.getWorkspacePath(), 
                                WorkspacePath.resolvePath(
                                    filter, 
                                    TestEnv.getWorkspacePath(), 
                                    TestEnv.getTestSuitePath()));
                            let filterAbsolute = "/" + path.relative(
                                TestEnv.getWorkspacePath(), 
                                filter);
                            if(testKeys[filterPathInSuite]) {
                                filteredTestCases = filteredTestCases.concat(testKeys[filterPathInSuite]);
                            } else if(testKeys[filterRelative]) {
                                filteredTestCases = filteredTestCases.concat(testKeys[filterRelative]);
                            } else if(testKeys[filterAbsolute]) {
                                filteredTestCases = filteredTestCases.concat(testKeys[filterAbsolute]);
                            }
                        }                        
                    }
                });
            }
            return resolve(filteredTestCases);
        });
    }


    static preprocessTestCases(p) {
        return new Promise((resolve, reject) => {
            let testCase = path.relative(TestEnv.getWorkspacePath(), p)
            let testCaseWoExt = path.join("/", path.dirname(testCase), path.basename(testCase, ".mjs"));
            TestCases.importTestCase(p).then(testCaseContent => {
                if(!Array.isArray(testCaseContent.default)) return reject(p + ": test case file validation failed");
                if(testCaseContent.default === 0) return reject(p + ": test case file validation failed");
                let testCasestype;
                testCaseContent.default.forEach(testCaseContentItem => {
                    if (!testCasestype) {
                        testCasestype = typeof testCaseContentItem;
                    } else {
                        if (testCasestype !== typeof testCaseContentItem) return reject(p + ": test case file validation failed");
                    }
                });
                if (testCasestype === "function") {
                    return resolve([
                        {
                            testFile: testCaseWoExt,
                            testType: "single",
                            testName: testCaseWoExt,
                            testIndex: undefined
                        }
                    ]);
                } else {
                    let testCaseContentItems = [];
                    testCaseContent.default.forEach((element, index) => {
                        testCaseContentItems.push({
                            testFile: testCaseWoExt,
                            testType: "multi",
                            testName: testCaseWoExt + "/" + element.testName,
                            testIndex: index,
                            errorMsg: element.errorMsg
                        })
                    });
                    return resolve(testCaseContentItems);
                }
            });
        });
    }


    static importTestCase(p) {
        return import(p);
    }
}


module.exports = TestCases;
