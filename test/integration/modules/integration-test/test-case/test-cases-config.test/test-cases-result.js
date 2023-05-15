const TestEnv = require("../../../../modules/integration-test/test-env.js");

const testCasesResult = {
  suites: [
    {
      config:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-case/test-cases-config.test/test-cases-1.json",
      suite:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-cases/test-suite-1",
      tests: {
        "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-1/example-1":
          {
            refs: ["1111111"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-1/example-2":
          {
            refs: ["2222222"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-3":
          {
            refs: ["3333333"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-4":
          {
            refs: ["4444444"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-5":
          {
            refs: ["5555555"],
          },
      },
    },
    {
      config:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-case/test-cases-config.test/test-cases-2.json",
      suite:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-cases/test-suite-2",
      tests: {
        "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-3/example-6":
          {
            refs: ["6666666"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-3/example-7":
          {
            refs: ["7777777"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-4/example-8":
          {
            refs: ["8888888"],
          },
        "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-4/example-9":
          {
            refs: ["9999999"],
          },
      },
    },
    {
      config:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-case/test-cases-config.test/test-cases-3.json",
      suite:
        TestEnv.getWorkspacePath() +
        "/test/integration/modules/integration-test/test-cases/test-suite-3",
      tests: {},
    },
  ],
  tests: {
    "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-1/example-1":
      {
        refs: ["1111111"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-1/example-2":
      {
        refs: ["2222222"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-3":
      {
        refs: ["3333333"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-4":
      {
        refs: ["4444444"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-1/example-group-2/example-5":
      {
        refs: ["5555555"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-3/example-6":
      {
        refs: ["6666666"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-3/example-7":
      {
        refs: ["7777777"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-4/example-8":
      {
        refs: ["8888888"],
      },
    "/test/integration/modules/integration-test/test-cases/test-suite-2/example-group-4/example-9":
      {
        refs: ["9999999"],
      },
  },
};

module.exports = testCasesResult;
