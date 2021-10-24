const fs = require('fs');

const webdriver = require('selenium-webdriver');
const chrome = require('selenium-webdriver/chrome');
const chromedriver = require('chromedriver');


class Chrome {

    #driver;
    

    constructor() {}


    openBrowser(headless=true) {
        /*const { Preferences, Type, Level } = require('selenium-webdriver/lib/logging')
        const { By, Key, Capabilities, until } = require('selenium-webdriver');
        const caps = webdriver.Capabilities.chrome();
        const logPrefs = new Preferences();
        logPrefs.setLevel(Type.BROWSER, Level.ALL);
        caps.setLoggingPrefs(logPrefs);
        caps.set('goog:loggingPrefs', logPrefs);
        const options = new chrome.Options(caps);*/

        //var service = new chrome.ServiceBuilder('/chromedriver/chromedriver').build();
        //chrome.setDefaultService(service);

        const builder = new webdriver.Builder();

        const options = new chrome.Options();

        const prefs = new webdriver.logging.Preferences();
        prefs.setLevel(webdriver.logging.Type.BROWSER, 
            webdriver.logging.Level.ALL);
        
        //options.setChromeBinaryPath('/usr/bin/google-chrome-stable')
        options.setLoggingPrefs(prefs);
        options.addArguments('force-device-scale-factor=1');
        options.addArguments('start-maximized');
        options.addArguments('--verbose');
        if (headless) {
            options.addArguments('--headless', '--no-sandbox', '--disable-dev-shm-usage');
        }
        this.#driver = builder
                .forBrowser('chrome')
                .setChromeOptions(options)
                .withCapabilities(webdriver.Capabilities.chrome())
                .build();

        /*this.#driver.manage().logs()
            .get(Type.BROWSER)
            .then(v => v && v.length && console.log(v));*/
    }

    closeBrowser(logPath) 
    {
        this.#driver.manage().logs().get(webdriver.logging.Type.BROWSER)
        .then((logs) => {
            if (logPath !== undefined) {
                for (let entry of logs) {
                    fs.appendFile(logPath, entry.message, function (err) {
                        if (err) {
                            throw err;
                        }
                    })
                }
            }
        })
        .then(() => { this.#driver.quit() });
    }
    

    getUrl(url) {
        return this.#driver.get(url);
    }

    executeScript(script) {
        return this.#driver.executeScript(script);
    }
}



module.exports = Chrome;
