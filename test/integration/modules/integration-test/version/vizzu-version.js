const path = require("path");

const WorkspaceHost = require("../../../modules/workspace/workspace-host.js");
const BrowserChrome = require("../../../modules/browser/chrome.js");
const VizzuUrl = require("../../../modules/integration-test/vizzu-url.js");
const TestEnv = require("../../../modules/integration-test/test-env.js");


class VizzuVersion {

    static get(vizzuUrl) {
        return new Promise((resolve, reject) => {
            let vizzuUrlReady = VizzuUrl.resolveVizzuUrl(vizzuUrl, TestEnv.getWorkspacePath(), TestEnv.getTestSuitePath());
            vizzuUrlReady.then(vizzuUrl => {
                let workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath());
                let workspaceHostReady = workspaceHost.serverPortReady();
                let browserChrome = new BrowserChrome();
                let browserChromeReady = browserChrome.initializing;
                Promise.all([workspaceHostReady, browserChromeReady]).then(([workspaceHostServerPort]) => {
                    if (vizzuUrl.startsWith("/")) {
                        vizzuUrl = "/" + path.relative(TestEnv.getWorkspacePath(), vizzuUrl);
                    }
                    browserChrome.getUrl("http://127.0.0.1:" + String(workspaceHostServerPort)
                        + "/test/integration/modules/integration-test/version/index.html"
                        + "?vizzuUrl=" + vizzuUrl)
                        .then(() => {
                            browserChrome.waitUntilTitleIs("Finished", 1000000).then(() => {
                                browserChrome.executeScript("return vizzuVersion").then(VizzuVersion => {
                                    browserChrome.closeBrowser();
                                    workspaceHost.closeServer();
                                    return resolve(VizzuVersion);
                                })
                            });
                        });
                });                
            });
        });
    }
}


module.exports = VizzuVersion;
