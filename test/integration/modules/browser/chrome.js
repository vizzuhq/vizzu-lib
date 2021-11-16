const fs = require('fs');

const webdriver = require('selenium-webdriver');
const chrome = require('selenium-webdriver/chrome');
const chromedriver = require('chromedriver');


class Chrome {

    #chromedriver;
    

    constructor(headless=true) {
        this.#startBrowser(headless);
    }


    #startBrowser(headless) {
        const builder = new webdriver.Builder();
        this.#chromedriver = builder
            .forBrowser('chrome')
            .setChromeOptions(this.#setBrowserOptions(headless))
            .withCapabilities(webdriver.Capabilities.chrome())
            .build();
    }


    closeBrowser(logPath) 
    {
        if (this.#chromedriver) {
            if (logPath) {
                this.#chromedriver.manage().logs().get(webdriver.logging.Type.BROWSER)
                .then((logs) => {
                    for (let entry of logs) {
                        fs.appendFile(logPath, entry.message, function (err) {
                            if (err) {
                                throw err;
                            }
                        })
                    }
                })
                .then(() => {
                    this.#chromedriver.quit();
                });
            } else {
                this.#chromedriver.quit();
            }
        }
    }


    #setBrowserOptions(headless) {
        const options = new chrome.Options();
        const prefs = new webdriver.logging.Preferences();
        prefs.setLevel(webdriver.logging.Type.BROWSER, 
            webdriver.logging.Level.ALL);
        options.setLoggingPrefs(prefs);
        options.addArguments('force-device-scale-factor=1');
        options.addArguments('start-maximized');
        options.addArguments('--verbose');
        if (headless) {
            options.addArguments('--headless', '--no-sandbox', '--disable-dev-shm-usage');
        }
        return options;
    }
    

    getUrl(url) {
        return this.#chromedriver.get(url);
    }


    executeScript(script) {
        return this.#chromedriver.executeScript(script);
    }


    waitUntilTitleIs(title, timeout) {
        return this.#chromedriver.wait(webdriver.until.titleIs(title), timeout);
    }

}


module.exports = Chrome;
