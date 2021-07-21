const path = require('path');


class Workspace {
    
    #workspacePath;
    #workspacePort;
    #workspace;

    constructor(workspacePath) {
        if(path.isAbsolute(workspacePath)) {
            this.#workspacePath = workspacePath;
        } else {
            this.#workspacePath = __dirname + '/' + workspacePath;
        }
    }


    getWorkspacePort() {
        return this.#workspacePort;
    }


    openWorkspace(port = this.#findFreePort()) {
        this.#workspacePort = port;
        let http = require('http');
        let finalhandler = require('finalhandler');
        let serveStatic = require('serve-static');
    
        let serve = serveStatic(this.#workspacePath);
        this.#workspace = http.createServer(function(req, res) {
            let done = finalhandler(req, res);
            serve(req, res, done);
        });
        this.#workspace.listen(port);
    }

    closeWorkspace() {
        this.#workspace.close();
    }


    #findFreePort() {
        let http = require('http');
    
        let server = http.createServer(function(req, res) {
            let done = finalhandler(req, res);
            serve(req, res, done);
        });
        server.listen(0, function() {});
        let port = server.address().port;
        server.close();
        return port;
    }
}


module.exports = Workspace;
