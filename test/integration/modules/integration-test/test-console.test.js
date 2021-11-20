const TestConsole = require("./test-console.js");
const TestEnv = require("./test-env.js");
const path = require("path");


describe("testSuiteLog", () => {
    test("if new TestConsole(), testSuiteLog is undefined", () => {
        let testConsole = new TestConsole();
        expect(testConsole.getTestSuiteLogPath()).toBeUndefined();
    });

    test("if new TestConsole(false), testSuiteLog is undefined", () => {
        let testConsole = new TestConsole(false);
        expect(testConsole.getTestSuiteLogPath()).toBeUndefined();
    });

    test("if new TestConsole(true), testSuiteLog is valid", () => {
        let testConsole = new TestConsole(true);
        expect(testConsole.getTestSuiteLogPath()).toBe(path.join(TestEnv.getTestSuiteReportPath(), "logs"));
    });
});

describe("testStatusPad", () => {
    test("if getTestStatusPad() returns 8", () => {
        let testConsole = new TestConsole();
        expect(testConsole.getTestStatusPad()).toBe(8);
    });

    test("if setTestStatusPad() err is thrown", () => {
        let testConsole = new TestConsole();
        expect(() => {
            testConsole.setTestStatusPad();
        }).toThrow("testStatusPad is integer");  
    });

    test("if setTestStatusPad(string) err is thrown", () => {
        let testConsole = new TestConsole();
        expect(() => {
            testConsole.setTestStatusPad("string");
        }).toThrow("testStatusPad is integer"); 
    });

    test("if setTestStatusPad(9), getTestStatusPad() returns 9", () => {
        let testConsole = new TestConsole();
        testConsole.setTestStatusPad(9);
        expect(testConsole.getTestStatusPad()).toBe(9);
    });
});

describe("testNumberPad", () => {
    test("if getTestNumberPad() returns 0", () => {
        let testConsole = new TestConsole();
        expect(testConsole.getTestNumberPad()).toBe(0);
    });

    test("if setTestNumberPad() err is thrown", () => {
        let testConsole = new TestConsole();
        expect(() => {
            testConsole.setTestNumberPad();
        }).toThrow("testNumberPad is integer");  
    });

    test("if setTestNumberPad(string) err is thrown", () => {
        let testConsole = new TestConsole();
        expect(() => {
            testConsole.setTestNumberPad("string");
        }).toThrow("testNumberPad is integer"); 
    });

    test("if setTestNumberPad(4), getTestNumberPad() returns 4", () => {
        let testConsole = new TestConsole();
        testConsole.setTestNumberPad(4);
        expect(testConsole.getTestNumberPad()).toBe(4);
    });
});

test("if log(\"Hello World\") logs \"Hello World\"", () => {
    let testConsole = new TestConsole();
    const msg = "Hello World";
    const somethingSpy = jest.spyOn(console, "log").mockImplementation(() => {});
    
    return testConsole.log(msg).then(() => {
        expect(somethingSpy).toBeCalledWith(msg);
    });
});

test("if getTimeStamp() is valid", () => {
    const startDate = new Date();
    let testConsole = new TestConsole();
    const timeStamp = testConsole.getTimeStamp();
    expect(timeStamp.length).toBe(15);
    const separator = timeStamp.substring(8,9);
    expect(separator).toBe("_");
    const year = timeStamp.substring(0,4);
    const month = timeStamp.substring(4,6) - 1;
    const day = timeStamp.substring(6,8);
    const hour = timeStamp.substring(9,11);
    const min = timeStamp.substring(11,13);
    const sec = timeStamp.substring(13,15);
    const timeStampDateMin = new Date(year, month, day, hour, min, sec, 0);
    const timeStampDateMax = new Date(year, month, day, hour, min, sec, 999);
    const endDate = new Date();
    expect(timeStampDateMin).toBeBeforeOrEqualTo(endDate);
    expect(timeStampDateMax).toBeAfterOrEqualTo(startDate);
});