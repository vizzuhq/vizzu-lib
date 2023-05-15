const path = require("path");
const fs = require("fs");
const Ajv = require("ajv");

const assert = require("../../../modules/console/assert.js");
const WorkspacePath = require("../../../modules/workspace/workspace-path.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");

class TestCasesConfig {
  static getConfig(configPathList) {
    return new Promise((resolve, reject) => {
      let configsReady = [];
      let configs = { suites: [], tests: {} };
      assert(Array.isArray(configPathList), "configPathList is array");
      let configPathListClone = configPathList.slice();
      configPathListClone.forEach((configPath, index) => {
        configPathListClone[index] = WorkspacePath.resolvePath(
          configPath,
          TestEnv.getWorkspacePath(),
          TestEnv.getTestSuitePath()
        );
        let configReady = new Promise((resolve, reject) => {
          TestCasesConfig.readConfig(configPathListClone[index])
            .then((config) => {
              assert(
                TestCasesConfig.isConfig(config),
                "config schema validation failed"
              );
              let suite = {
                suite: path.join(TestEnv.getWorkspacePath(), config.data.suite),
                config: config.path,
                tests: {},
              };
              if (config.data.test) {
                Object.keys(config.data.test).forEach((testCase) => {
                  let testCaseId = path.join(config.data.suite, testCase);
                  let testCaseData = config.data.test[testCase];
                  suite.tests[testCaseId] = testCaseData;
                  configs.tests[testCaseId] = testCaseData;
                });
              }
              configs.suites.push(suite);
              return resolve();
            })
            .catch((err) => {
              return reject(err);
            });
        });
        configsReady.push(configReady);
      });
      Promise.all(configsReady)
        .then(() => {
          return resolve(configs);
        })
        .catch((err) => {
          return reject(err);
        });
    });
  }

  static readConfig(configPath) {
    return new Promise((resolve, reject) => {
      fs.stat(configPath, (err) => {
        if (err === null) {
          fs.readFile(configPath, (err, data) => {
            if (err) {
              return reject(err);
            }
            try {
              data = JSON.parse(data);
            } catch (err) {
              return reject(err);
            }
            return resolve({ path: configPath, data: data });
          });
        } else {
          return reject(err);
        }
      });
    });
  }

  static isConfig(config) {
    const validate = new Ajv().compile(TestCasesConfig.getConfigSchema());
    return validate(config);
  }

  static getConfigSchema() {
    return {
      type: "object",
      properties: {
        path: {
          type: "string",
        },
        data: {
          type: "object",
          properties: {
            suite: {
              type: "string",
            },
            test: {
              type: "object",
              additionalProperties: {
                type: "object",
                properties: {
                  refs: {
                    type: "array",
                  },
                },
                additionalProperties: false,
              },
            },
          },
          required: ["suite"],
          additionalProperties: false,
        },
      },
      required: ["path", "data"],
      additionalProperties: false,
    };
  }

  static isTestCasesConfig(testCasesConfig) {
    const validate = new Ajv().compile(
      TestCasesConfig.getTestCasesConfigSchema()
    );
    return validate(testCasesConfig);
  }

  static getTestCasesConfigSchema() {
    return {
      type: "object",
      properties: {
        suites: {
          type: "array",
          items: {
            type: "object",
            properties: {
              suite: {
                type: "string",
              },
              config: {
                type: "string",
              },
              tests: {
                type: "object",
                additionalProperties: {
                  type: "object",
                  properties: {
                    refs: {
                      type: "array",
                    },
                  },
                  additionalProperties: false,
                },
              },
            },
            required: ["suite", "config", "tests"],
            additionalProperties: false,
          },
        },
        tests: {
          type: "object",
          additionalProperties: {
            type: "object",
            properties: {
              refs: {
                type: "array",
              },
            },
            additionalProperties: false,
          },
        },
      },
      required: ["suites", "tests"],
      additionalProperties: false,
    };
  }
}

module.exports = TestCasesConfig;
