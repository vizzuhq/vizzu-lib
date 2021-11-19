const path = require('path');
const serveStatic = require('serve-static')
const express = require('express');


class WorkspaceHost {

    #server;
    #serverPort;


    constructor(workspacePath, port=0) {
        this.#serverPort = this.#createServer(workspacePath, port);
    }


    serverPortReady() {
        return this.#serverPort;
    }


    #createServer(workspacePath, port) {
        return new Promise((resolve, reject) => {
            let app = express();
            app.use(serveStatic(this.#resolveWorkspacePath(workspacePath)));
            this.#server = app.listen(port, () => {
                return resolve(this.#server.address().port);
            });
        });
    }


    closeServer() {
        if (this.#server) {
            this.#server.close();
        }
    }


    #resolveWorkspacePath(workspacePath) {
        if(path.isAbsolute(workspacePath)) {
            return workspacePath;
        } else {
            return __dirname + '/' + workspacePath;
        }
    }

}


module.exports = WorkspaceHost;
