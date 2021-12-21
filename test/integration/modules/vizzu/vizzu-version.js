const path = require("path");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const BrowserChrome = require("../../modules/browser/chrome.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const TestEnv = require("../../modules/integration-test/test-env.js");


class VizzuVersion {

    static getVizzuUrlVersion(vizzuUrl) {
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
                        + "/test/integration/modules/vizzu/vizzu-version-client/index.html"
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

    static getPrivateBetaList() {
        return [
            {num: "0.2.0", url: "https://vizzuhq.github.io/vizzu-beta-release/0.2.0"}
        ];
    }

    static getPublicBetaList() {
        let publicBetaList = [
            "0.3.0",
            "0.3.1",
            "0.3.2",
            "0.3.3",
            "0.4.0",
            "0.4.1"
        ];
        publicBetaList.slice().reverse().forEach((version, index) => {
            publicBetaList[index] = {num: version, url: VizzuUrl.getRemoteCdn() + "@" + version + "/dist/vizzu.min.js"};
        });
        return publicBetaList;
    }
}


module.exports = VizzuVersion;
