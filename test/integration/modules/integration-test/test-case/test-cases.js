const path = require("path");
const fs = require("fs");

const WorkspacePath = require("../../../modules/workspace/workspace-path.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");


class TestCases {

    static getTestCases(testCasesConfigReady, filters) {
        return new Promise((resolve, reject) => {
            testCasesConfigReady.then(configs => {
                let testCasesReadyList = [];
                let filteredTestCasesReadyList = [];
                configs.suites.forEach(suite => {
                    let testCasesReady = TestCases.collectTestCases(suite.suite);
                    testCasesReadyList.push(testCasesReady);
                    testCasesReady.then(testCases => {
                        let filteredTestCasesReady = TestCases.filterTestCases(testCases, suite.suite, filters);
                        filteredTestCasesReadyList.push(filteredTestCasesReady);
                    });
                });
                Promise.all(testCasesReadyList).then(testCasesList => {
                    testCasesList = testCasesList.flat(1);
                    Promise.all(filteredTestCasesReadyList).then(filteredTestCasesList => {
                        filteredTestCasesList = filteredTestCasesList.flat(1);
                        return resolve({testCases: testCasesList, filteredTestCases: filteredTestCasesList});
                    });
                });
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
                                    });
                                });
                                Promise.all(testCasesReady).then(() => {
                                    return resolve(testCases);
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


    static filterTestCases(testCases, suitePath, filters) {
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
                            let filterWithSuitePath = path.join(suitePath, filter);
                            let filterRelative = "/" + path.relative(
                                TestEnv.getWorkspacePath(), 
                                WorkspacePath.resolvePath(
                                    filter, 
                                    TestEnv.getWorkspacePath(), 
                                    TestEnv.getTestSuitePath()));
                            let filterAbsolute = "/" + path.relative(
                                TestEnv.getWorkspacePath(), 
                                filter);
                            if(testCases.includes(filterWithSuitePath)) {
                                filteredTestCases.push(filterWithSuitePath);
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
