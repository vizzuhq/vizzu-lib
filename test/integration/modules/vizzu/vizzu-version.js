const path = require("path");

const fetch = require("node-fetch");

const WorkspaceHost = require("../../modules/workspace/workspace-host.js");
const BrowserChrome = require("../../modules/browser/chrome.js");
const VizzuUrl = require("../../modules/vizzu/vizzu-url.js");
const TestEnv = require("../../modules/integration-test/test-env.js");

class VizzuVersion {
  static getVizzuUrlVersion(vizzuUrl) {
    return new Promise((resolve, reject) => {
      let vizzuUrlReady = VizzuUrl.resolveVizzuUrl(
        vizzuUrl,
        TestEnv.getWorkspacePath(),
        TestEnv.getTestSuitePath()
      );
      vizzuUrlReady.then((vizzuUrl) => {
        let workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath());
        let workspaceHostReady = workspaceHost.serverPortReady();
        let browserChrome = new BrowserChrome();
        let browserChromeReady = browserChrome.initializing;
        Promise.all([workspaceHostReady, browserChromeReady]).then(
          ([workspaceHostServerPort]) => {
            if (vizzuUrl.startsWith("/")) {
              vizzuUrl =
                "/" + path.relative(TestEnv.getWorkspacePath(), vizzuUrl);
            }
            browserChrome
              .getUrl(
                "http://127.0.0.1:" +
                  String(workspaceHostServerPort) +
                  "/test/integration/modules/vizzu/vizzu-version-client/index.html" +
                  "?vizzuUrl=" +
                  vizzuUrl
              )
              .then(() => {
                browserChrome.waitUntilTitleIs("Finished", 30000).then(() => {
                  browserChrome
                    .executeScript("return vizzuVersion")
                    .then((VizzuVersion) => {
                      browserChrome.closeBrowser();
                      workspaceHost.closeServer();
                      return resolve(VizzuVersion);
                    });
                });
              });
          }
        );
      });
    });
  }

  static getPrivateBetaList() {
    return [
      {
        num: "0.2.0 (private beta)",
        url: "https://vizzuhq.github.io/vizzu-beta-release/0.2.0",
      },
    ];
  }

  static getPublicBetaList() {
    return new Promise((resolve) => {
      fetch("https://data.jsdelivr.com/v1/package/npm/vizzu")
        .then((data) => {
          return data.json().then((data) => {
            let publicBetaList = data.versions;
            publicBetaList.slice().forEach((version, index) => {
              publicBetaList[index] = {
                num: version + " (cdn)",
                url:
                  VizzuUrl.getRemoteCdn() +
                  "@" +
                  version +
                  "/dist/vizzu.min.js",
              };
            });
            return resolve(publicBetaList);
          });
        })
        .catch((err) => {
          console.error("failed to fetch cdn lib list");
          return resolve([]);
        });
    });
  }

  static isUrlAvailable(url) {
    return new Promise((resolve, reject) => {
      fetch(url, { method: 'HEAD' })
        .then((response) => {
          resolve(response.status === 200);
        })
        .catch((error) => {
          resolve(false);
        });
    });
  }
}

module.exports = VizzuVersion;
