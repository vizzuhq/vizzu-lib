const WorkspaceHost = require("../../../modules/workspace/workspace-host.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");

const LibsHandler = require("./handlers/libs.js");
const TestsHandler = require("./handlers/tests.js");
const TestCaseHandler = require("./handlers/test-case.js");

class Manual {
  #workspaceHost;
  #workspaceHostReady;
  #workspaceHostServerPort;

  #configPathList;
  #filters;

  constructor(configPathList, filters, workspaceHostServerPort) {
    this.#configPathList = configPathList;
    this.#filters = filters;
    this.#workspaceHostServerPort = workspaceHostServerPort;
  }

  run() {
    this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath(), this.#workspaceHostServerPort);
    this.#workspaceHostReady = this.#workspaceHost.serverPortReady();

    this.#workspaceHostReady.then(() => {
      this.#setRouteGetLibs();
      this.#setRouteGetTests();
      this.#setRouteValidateTestCase();

      console.log(`[ W. HOST ] [ http://127.0.0.1:${this.#workspaceHostServerPort}/test/integration/tools/manual/client ] press CTRL + C to stop`);
    });
  }

  #setRouteGetLibs() {
    this.#workspaceHost.setRoute("/getLibs", (req, res) => {
      const libsHandler = new LibsHandler(res, this.#workspaceHostServerPort);
      libsHandler.handle();
    });
  }

  #setRouteGetTests() {
    this.#workspaceHost.setRoute("/getTests", (req, res) => {
      const testsHandler = new TestsHandler(res, this.#configPathList, this.#filters);
      testsHandler.handle();
    });
  }

  #setRouteValidateTestCase() {
    this.#workspaceHost.setPostRoute("/validateTestCase", (req, res) => {
      const testCaseHandler = new TestCaseHandler(req, res);
      testCaseHandler.handle();
    });
  }
}

module.exports = Manual;
