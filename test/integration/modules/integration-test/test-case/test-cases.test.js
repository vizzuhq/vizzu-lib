const path = require("path");
const fs = require("fs");

const TestEnv = require("../test-env.js");
const TestCases = require("./test-cases.js");

const suites = path.join(__dirname, "test-cases");
const testPathList = [
  path.join(suites, "suite1", "notest1.js"),
  path.join(suites, "suite1", "test1.mjs"),
  path.join(suites, "suite1", "testgroup1a", "test1a.mjs"),
  path.join(suites, "suite1", "testgroup1a", "test1aa.mjs"),
  path.join(suites, "suite1", "testgroup1a", "testgroup1b", "test1b.mjs"),
  path.join(suites, "suite1", "testgroup1a", "testgroup1b", "test1bb.mjs"),
  path.join(
    suites,
    "suite1",
    "testgroup1a",
    "testgroup1b",
    "testgroup1c",
    "test1c.mjs"
  ),
  path.join(suites, "suite2", "test2.mjs"),
  path.join(suites, "suite2", "testgroup2a", "test2a.mjs"),
  path.join(suites, "suite2", "testgroup2a", "test2aa.mjs"),
  path.join(suites, "suite2", "testgroup2a", "testgroup2b", "test2b.mjs"),
  path.join(suites, "suite2", "testgroup2a", "testgroup2b", "test2bb.mjs"),
  path.join(
    suites,
    "suite2",
    "testgroup2a",
    "testgroup2b",
    "testgroup2c",
    "test2c.mjs"
  ),
  path.join(suites, "suite2", "testgroup2d", "test2d.mjs"),
  path.join(suites, "suite2", "testgroup2d", "notest2.js"),
];

const testCaseList = [
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/test1",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/test1",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/test1a",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/test1a",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/test1aa",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/test1aa",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1b",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1b",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/testgroup1c/test1c",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/testgroup1c/test1c",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/test2",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/test2",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/test2a",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/test2a",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/test2aa",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/test2aa",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/test2b",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/test2b",
    testType: "single",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/test2bb",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/test2bb",
    testType: "single",
  },
  {
    errorMsg: undefined,
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
    testIndex: 0,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c1",
    testType: "multi",
  },
  {
    errorMsg: undefined,
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
    testIndex: 1,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c2",
    testType: "multi",
  },
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2d/test2d",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2d/test2d",
    testType: "single",
  },
];

const filteredTestCaseList = [
  {
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
    testIndex: undefined,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
    testType: "single",
  },
  {
    errorMsg: undefined,
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
    testIndex: 0,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c1",
    testType: "multi",
  },
  {
    errorMsg: undefined,
    testFile:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
    testIndex: 1,
    testName:
      "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c2",
    testType: "multi",
  },
];

const testCasesConfigReady = new Promise((resolve, reject) => {
  return resolve({
    suites: [
      {
        suite: "./modules/integration-test/test-case/test-cases/suite1",
        config: "",
        tests: {},
      },
      {
        suite: "./modules/integration-test/test-case/test-cases/suite2",
        config: "",
        tests: {},
      },
    ],
    tests: {},
  });
});

beforeEach(() => {
  jest.useFakeTimers();
  jest.resetAllMocks();
  jest.spyOn(TestCases, "importTestCase").mockImplementation((p) => {
    if (
      p ===
      "modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c.mjs"
    ) {
      return new Promise((resolve, reject) => {
        return resolve({
          default: [
            {
              testName: "c1",
              testSteps: [() => {}],
            },
            {
              testName: "c2",
              testSteps: [() => {}],
            },
          ],
        });
      });
    } else {
      return new Promise((resolve, reject) => {
        return resolve({ default: [() => {}] });
      });
    }
  });
});

beforeAll(() => {
  return new Promise((resolve, reject) => {
    let testsReady = [];
    testPathList.forEach((testPath) => {
      let testReady = new Promise((resolve, reject) => {
        fs.mkdir(
          path.dirname(testPath),
          { force: true, recursive: true },
          (err) => {
            if (err) {
              throw err;
            }
            fs.open(testPath, "w", (err) => {
              if (err) {
                throw err;
              }
              return resolve();
            });
          }
        );
      });
      testsReady.push(testReady);
    });
    Promise.all(testsReady).then(() => {
      return resolve();
    });
  });
});

describe("getTestCases()", () => {
  describe("testCasesConfigReady", () => {
    test("if undefined, err is thrown", () => {
      return expect(TestCases.getTestCases()).rejects.toThrow(
        "Cannot read properties of undefined (reading 'then')"
      );
    });

    test("if not a promise, err is thrown", () => {
      return expect(
        TestCases.getTestCases("testCasesConfigReady")
      ).rejects.toThrow("testCasesConfigReady.then is not a function");
    });

    test("if does not fit into schema, err is thrown", () => {
      let testCasesConfigReady = new Promise((resolve, reject) => {
        return resolve({});
      });
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toThrow(
        "Assert failed: test cases config schema validation failed"
      );
    });

    test("if fits into schema, test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: testCaseList,
        });
      });
    });

    test("if suite does not exist, err is thrown", () => {
      const wrongTestCasesConfigReadyENOENT = new Promise((resolve, reject) => {
        return resolve({
          suites: [
            {
              suite: "./modules/integration-test/test-case/test-cases/suite3",
              config: "",
              tests: {},
            },
          ],
          tests: {},
        });
      });
      return expect(
        TestCases.getTestCases(wrongTestCasesConfigReadyENOENT)
      ).rejects.toThrow(
        "ENOENT: no such file or directory, lstat './modules/integration-test/test-case/test-cases/suite3'"
      );
    });

    test("if suite root does not have permission, err is thrown", () => {
      const wrongTestCasesConfigReadyEACCES = new Promise((resolve, reject) => {
        return resolve({
          suites: [{ suite: "/root", config: "", tests: {} }],
          tests: {},
        });
      });
      return expect(
        TestCases.getTestCases(wrongTestCasesConfigReadyEACCES)
      ).rejects.toThrow("EACCES: permission denied, scandir '/root'");
    });

    test("if suite item does not have permission1, err is thrown", () => {
      const wrongTestCasesConfigReadyEACCES1 = new Promise(
        (resolve, reject) => {
          return resolve({
            suites: [{ suite: "/var/log", config: "", tests: {} }],
            tests: {},
          });
        }
      );
      return expect(
        TestCases.getTestCases(wrongTestCasesConfigReadyEACCES1)
      ).rejects.toThrow("EACCES: permission denied, scandir");
    });

    test("if test file is not array, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: undefined });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });

    test("if test file is an empty array, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: [] });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });

    test("if test case type is not an object or function, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: ["string"] });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });

    test("if test case type is null, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: [null] });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });

    test("if test case type is an array, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: [[]] });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });

    test("if test case types are different, err is thrown", () => {
      jest.spyOn(TestCases, "importTestCase").mockReturnValue(
        new Promise((resolve, reject) => {
          return resolve({ default: [() => {}, {}] });
        })
      );
      return expect(
        TestCases.getTestCases(testCasesConfigReady)
      ).rejects.toContain("test case file validation failed");
    });
  });

  describe("filters", () => {
    test("if name, filtered test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady, [
        "test1bb",
        "test2c.mjs",
      ]).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: filteredTestCaseList,
        });
      });
    });

    test("if path (in suite), filtered test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady, [
        "testgroup1a/testgroup1b/test1bb",
        "testgroup2a/testgroup2b/testgroup2c/test2c",
      ]).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: filteredTestCaseList,
        });
      });
    });

    test("if absolute path, filtered test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady, [
        path.join(
          TestEnv.getWorkspacePath(),
          "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb"
        ),
        path.join(
          TestEnv.getWorkspacePath(),
          "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c"
        ),
      ]).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: filteredTestCaseList,
        });
      });
    });

    test("if absolute path (workspace), filtered test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady, [
        "/test/integration/modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
        "/test/integration/modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
      ]).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: filteredTestCaseList,
        });
      });
    });

    test("if relative path (workspace), filtered test cases are valid", () => {
      return TestCases.getTestCases(testCasesConfigReady, [
        "./modules/integration-test/test-case/test-cases/suite1/testgroup1a/testgroup1b/test1bb",
        "modules/integration-test/test-case/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c",
      ]).then((testCases) => {
        testCases.testCases = testCases.testCases;
        testCases.filteredTestCases = testCases.filteredTestCases;
        expect(testCases).toEqual({
          testCases: testCaseList,
          filteredTestCases: filteredTestCaseList,
        });
      });
    });
  });
});

afterAll(() => {
  fs.rm(suites, { force: true, recursive: true }, (err) => {
    if (err) {
      throw err;
    }
  });
});
