const path = require('path')

const fetch = require('node-fetch')

const WorkspaceHost = require('../../modules/workspace/workspace-host.cjs')
const BrowserChrome = require('../../modules/browser/puppeteer-chrome.cjs')
const VizzuUrl = require('../../modules/vizzu/vizzu-url.cjs')
const TestEnv = require('../../modules/e2e-test/test-env.cjs')

class VizzuVersion {
	static getVizzuUrlVersion(vizzuUrl) {
		return new Promise((resolve, reject) => {
			const vizzuUrlReady = VizzuUrl.resolveVizzuUrl(
				vizzuUrl,
				TestEnv.getWorkspacePath(),
				TestEnv.getTestSuitePath()
			)
			vizzuUrlReady.then((vizzuUrl) => {
				const workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath())
				const workspaceHostReady = workspaceHost.serverPortReady()
				const browserChrome = new BrowserChrome()
				const browserChromeReady = browserChrome.initializing
				Promise.all([workspaceHostReady, browserChromeReady]).then(
					([workspaceHostServerPort]) => {
						if (vizzuUrl.startsWith('/')) {
							vizzuUrl = '/' + path.relative(TestEnv.getWorkspacePath(), vizzuUrl)
						}
						browserChrome
							.getUrl(
								'http://127.0.0.1:' +
									String(workspaceHostServerPort) +
									'/test/e2e/modules/vizzu/vizzu-version-client/index.html' +
									'?vizzuUrl=' +
									vizzuUrl
							)
							.then(() => {
								browserChrome.waitUntilTitleIs('Finished', 30000).then(() => {
									browserChrome
										.executeScript(() => {
											return vizzuVersion // eslint-disable-line no-undef
										})
										.then((VizzuVersion) => {
											browserChrome.closeBrowser()
											workspaceHost.closeServer()
											return resolve(VizzuVersion)
										})
								})
							})
					}
				)
			})
		})
	}

	static getPrivateBetaList() {
		return [
			{
				num: '0.2.0 (private beta)',
				url: 'https://vizzuhq.github.io/vizzu-beta-release/0.2.0'
			}
		]
	}

	static getPublicBetaList() {
		return new Promise((resolve) => {
			fetch('https://data.jsdelivr.com/v1/package/npm/vizzu')
				.then((data) => {
					return data.json().then((data) => {
						const publicBetaList = data.versions
						publicBetaList.slice().forEach((version, index) => {
							publicBetaList[index] = {
								num: version + ' (cdn)',
								url: VizzuUrl.getRemoteCdn() + '@' + version + '/dist/vizzu.min.js'
							}
						})
						return resolve(publicBetaList)
					})
				})
				.catch((err) => {
					console.error(err)
					console.error('failed to fetch cdn lib list')
					return resolve([])
				})
		})
	}

	static checkUrlAvailability(url) {
		return fetch(url, { method: 'HEAD' })
			.then((response) => {
				if (response.status !== 200) {
					throw new Error(`failed to fetch url: ${response.status}`)
				}
			})
			.catch((error) => {
				throw error
			})
	}
}

module.exports = VizzuVersion
