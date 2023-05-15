const path = require("path");
const serveStatic = require("serve-static");
const express = require("express");

class WorkspaceHost {
  #app;

  #server;
  #serverPort;

  constructor(workspacePath, port = 0) {
    this.#serverPort = this.#startServer(workspacePath, port);
  }

  serverPortReady() {
    return this.#serverPort;
  }

  #startServer(workspacePath, port) {
    return new Promise((resolve, reject) => {
      this.#app = express();
      this.#app.use(serveStatic(this.#resolveWorkspacePath(workspacePath)));
      this.#server = this.#app.listen(port, () => {
        return resolve(this.#server.address().port);
      });
    });
  }

  setRoute(path, handler) {
    this.#app.get(path, handler);
  }

  closeServer() {
    if (this.#server) {
      this.#server.close();
    }
  }

  #resolveWorkspacePath(workspacePath) {
    if (path.isAbsolute(workspacePath)) {
      return workspacePath;
    } else {
      return __dirname + "/" + workspacePath;
    }
  }
}

module.exports = WorkspaceHost;
