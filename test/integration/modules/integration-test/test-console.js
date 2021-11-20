const path = require("path");

const TestEnv = require("../../modules/integration-test/test-env.js");
const Console = require("../../modules/console/console.js");


class TestConsole {

    #console;

    #filePrefix;
    #pathPrefix;
    #testSuiteLogPath;
    
    #testStatusPad = 8;
    #testNumberPad = 0;


    constructor(fileLog) {
        if (fileLog) {
            this.#filePrefix = "integration.test";
            this.#pathPrefix = "logs";
            this.#testSuiteLogPath = path.join(TestEnv.getTestSuiteReportPath(), this.#pathPrefix);
        }
        this.#console = new Console(this.#filePrefix, this.#testSuiteLogPath);
    }


    log(msg) {
        return this.#console.log(msg);
    }


    getTimeStamp() {
        return this.#console.getTimeStamp();
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
