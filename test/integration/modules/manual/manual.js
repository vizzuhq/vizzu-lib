const fetch = require("node-fetch");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const VizzuCloudFunctions = require("../../modules/vizzu/vizzu-cloudfunctions.js");
const VizzuVersion = require("../../modules/vizzu/vizzu-version.js");
const TestEnv = require("../../modules/integration-test/test-env.js");
const TestCasesConfig = require("../../modules/integration-test/test-case/test-cases-config.js");
const TestCases = require("../../modules/integration-test/test-case/test-cases.js");


class Manual {

    #workspaceHost;
    #workspaceHostReady;
    #workspaceHostServerPort;

    #configPathList


    constructor(
        configPathList,
        workspaceHostServerPort
    ) {
        this.#configPathList = configPathList;
        this.#workspaceHostServerPort = workspaceHostServerPort;
    }


    runManual() {
        this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath(), this.#workspaceHostServerPort);
        this.#workspaceHostReady = this.#workspaceHost.serverPortReady();
        this.#workspaceHostReady.then(() => {
            this.#setRouteGetLibList();
            this.#setRouteGetTestList();
            console.log("[ " + "W. HOST" + " ]" + " " + "[ " + "http://127.0.0.1:" + String(this.#workspaceHostServerPort) + "/test/integration/modules/manual/client" + " ]" + " press CTRL + C to stop");
        });
    }


    #setRouteGetLibList() {
        this.#workspaceHost.setRoute("/getLibList", (req, res) => {
            let libList = { localhost: "http://127.0.0.1:" + this.#workspaceHostServerPort + "/example/lib" };
            libList["HEAD"] = VizzuUrl.getRemoteStableBucket() + "/lib";
            this.#fetchLibList().then(vizzuList => {
                vizzuList.slice().reverse().forEach(vizzu => {
                    libList[vizzu.time.substring(0,10) + " " + vizzu.time.substring(11,16)  + " " + vizzu.sha] = VizzuUrl.getRemoteBucket() + "/" + vizzu.sha;
                });
                VizzuVersion.getPublicBetaList().forEach(version => {
                    libList[version.num] = version.url;
                });
                VizzuVersion.getPrivateBetaList().forEach(version => {
                    libList[version.num] = version.url;
                });
                res.send(libList);
            });
        });
    }


    #fetchLibList() {
        return new Promise(resolve => {
            fetch(VizzuCloudFunctions.getRemoteCloudFunctions() + "/getVizzuList").then(vizzuListUrl => {
                vizzuListUrl.json().then(vizzuList => {
                    return resolve(vizzuList);
                }).catch((err) => {
                    return resolve(this.#fetchLibList());
                });
            });
        });
    }


    #setRouteGetTestList() {
        this.#workspaceHost.setRoute("/getTestList", (req, res) => {
            let testCasesConfigReady = TestCasesConfig.getConfig(this.#configPathList);
            TestCases.getTestCases(testCasesConfigReady, []).then(testCases => {
                res.send(testCases.filteredTestCases);
            });
        });
    }
}

module.exports = Manual;
