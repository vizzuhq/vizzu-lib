const fs = require('fs')

const webdriver = require('selenium-webdriver')
const chrome = require('selenium-webdriver/chrome')

class SeleniumChrome {
	initializing

	#chromedriver

	constructor(headless = true) {
		this.initializing = this.#startBrowser(headless)
	}

	#startBrowser(headless) {
		return new Promise((resolve, reject) => {
			const builder = new webdriver.Builder()
			this.#chromedriver = builder
				.forBrowser('chrome')
				.setChromeOptions(this.#setBrowserOptions(headless))
				.withCapabilities(webdriver.Capabilities.chrome())
				.build()
			this.#chromedriver
				.then(() => {
					return resolve()
				})
				.catch((err) => {
					return reject(err)
				})
		})
	}

	closeBrowser(browserLog) {
		if (this.#chromedriver) {
			this.initializing
				.then(() => {
					let browserLogReady = new Promise((resolve) => {
						resolve()
					})
					if (browserLog) {
						browserLogReady = new Promise((resolve, reject) => {
							this.#chromedriver
								.manage()
								.logs()
								.get(webdriver.logging.Type.BROWSER)
								.then((logs) => {
									for (const entry of logs) {
										fs.appendFile(browserLog, entry.message, function (err) {
											if (err) {
												return reject(err)
											}
										})
									}
									return resolve()
								})
						})
					}
					browserLogReady.then(() => {
						this.#chromedriver.quit().catch((err) => {
							const errMsg = err.toString()
							if (!errMsg.includes('ECONNREFUSED connect ECONNREFUSED')) {
								throw err
							}
						})
					})
				})
				.catch((err) => {
					console.log(err)
				})
		}
	}

	#setBrowserOptions(headless) {
		const options = new chrome.Options()
		const prefs = new webdriver.logging.Preferences()
		prefs.setLevel(webdriver.logging.Type.BROWSER, webdriver.logging.Level.ALL)
		options.setLoggingPrefs(prefs)
		options.addArguments('force-device-scale-factor=1')
		options.addArguments('start-maximized')
		options.addArguments('--verbose')
		if (headless) {
			options.addArguments('--headless', '--no-sandbox', '--disable-dev-shm-usage')
		}
		return options
	}

	getUrl(url) {
		return this.#chromedriver.get(url)
	}

	executeScript(script) {
		return this.#chromedriver.executeScript(script)
	}

	waitUntilTitleIs(title, timeout) {
		return this.#chromedriver.wait(webdriver.until.titleIs(title), timeout)
	}
}

module.exports = SeleniumChrome
