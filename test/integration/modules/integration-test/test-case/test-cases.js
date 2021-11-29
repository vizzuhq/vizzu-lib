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
                            let testCase = path.relative(TestEnv.getWorkspacePath(), p)
                            let testCaseWoExt = path.join("/", path.dirname(testCase), path.basename(testCase, ".mjs"));
                            return resolve([testCaseWoExt]);
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
                filters.forEach(filter => {
                    filter = path.join(path.dirname(filter), path.basename(filter, ".mjs"));
                    if (path.dirname(filter) === ".") {
                        testCases.forEach(testCase => {
                            if (path.basename(filter) === path.basename(testCase)) {
                                filteredTestCases.push(testCase);
                            }
                        });
                    } else {
                        if (testCases.includes(filter)) {
                            filteredTestCases.push(filter);
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
                            if(testCases.includes(filterPathInSuite)) {
                                filteredTestCases.push(filterPathInSuite);
                            } else if(testCases.includes(filterRelative)) {
                                filteredTestCases.push(filterRelative);
                            } else if(testCases.includes(filterAbsolute)) {
                                filteredTestCases.push(filterAbsolute);
                            }
                        }                        
                    }
                });
            }
            return resolve(filteredTestCases);
        });
    }
}


module.exports = TestCases;
