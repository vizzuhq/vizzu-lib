const path = require("path");

const TestEnv = require("../../modules/integration-test/test-env.js");
const Console = require("../../modules/console/console.js");


class TestConsole extends Console {

    #testSuiteLogPath;
    
    #testStatusPad = 8;
    #testNumberPad = 0;


    constructor(fileLog) {
        if (fileLog) {
            var filePrefix = "integration.test";
            var pathPrefix = "logs";
            var testSuiteLogPath = path.join(TestEnv.getTestSuiteReportPath(), pathPrefix);
        }
        super(filePrefix, testSuiteLogPath);
        this.#testSuiteLogPath = testSuiteLogPath;
    }


    getTestSuiteLogPath() {
        return this.#testSuiteLogPath;
    }


    getTestStatusPad() {
        return this.#testStatusPad;
    }


    getTestNumberPad() {
        return this.#testNumberPad;
    }


    setTestStatusPad(testStatusPad) {
        testStatusPad = parseInt(testStatusPad);
        if (isNaN(testStatusPad)) {
            throw new Error("testStatusPad is integer");
        }
        this.#testStatusPad = testStatusPad;
    }


    setTestNumberPad(testNumberPad) {
        testNumberPad = parseInt(testNumberPad);
        if (isNaN(testNumberPad)) {
            throw new Error("testNumberPad is integer");
        }
        this.#testNumberPad = testNumberPad;
    }
}


module.exports = TestConsole;
