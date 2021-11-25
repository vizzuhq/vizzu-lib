const TestCasesConfig = require("./test-cases-config.js");

const testCasesResult = require("./test-cases-config.test/test-cases-result.js");


describe("getConfig()", () => {
    describe("configPathList", () => {
        test("if configPathList is no an array, err is thrown", () => {
            return TestCasesConfig.getConfig().catch(err => {
                expect(err.toString()).toBe("Error: Assert failed: configPathList is array");
            });
        });

        test("if configPathList is an empty array", () => {
            return TestCasesConfig.getConfig([]).then(config => {
                expect(config).toStrictEqual({"suites": [], "tests": {}});
            });
        });

        test("if configPathList is an array", () => {
            return TestCasesConfig.getConfig([
                "./modules/integration-test/test-case/test-cases-config.test/test-cases-1.json",
                "./modules/integration-test/test-case/test-cases-config.test/test-cases-2.json"
            ]).then(config => {
                expect(config).toStrictEqual(testCasesResult);
            });
        });
    });

    describe("configPath", () => {
        test("if configPath name is not string, err is thrown", () => {
            return TestCasesConfig.getConfig([
                []
            ]).catch(err => {
                expect(err.toString()).toInclude("TypeError [ERR_INVALID_ARG_TYPE]: The \"path\" argument must be of type string. Received an instance of Array");
            });
        });

        test("if configPath is not file, err is thrown", () => {
            return TestCasesConfig.getConfig([
                "./modules/integration-test/test-case/test-cases-config.test"
            ]).catch(err => {
                expect(err.toString()).toInclude("Error: EISDIR: illegal operation on a directory, read");
            });
        });

        test("if configPath does not exist, err is thrown", () => {
            return TestCasesConfig.getConfig([
                "./modules/integration-test/test-case/test-cases-config.test/test-cases-4.json"
            ]).catch(err => {
                expect(err.toString()).toInclude("Error: ENOENT: no such file or directory");
            });
        });

        test("if configPath is not json, err is thrown", () => {
            return TestCasesConfig.getConfig([
                "./modules/integration-test/test-case/test-cases-config.test/test-cases-3.json"
            ]).catch(err => {
                expect(err.toString()).toInclude("SyntaxError: Unexpected end of JSON input");
            });
        });
    });
});
