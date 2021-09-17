const { resolve } = require('path');

const remoteLatestBucket = 'vizzu-lib-main-sha.storage.googleapis.com';
const remoteStableBucket = 'vizzu-lib-main.storage.googleapis.com';
const defaultTestCaseTimeout = 60000;
const padLength = 7;


try {

    const AggregateError = import('aggregate-error');

    const fs = require('fs');
    const path = require('path');
    const yargs = require('yargs');
    const fetch = require('node-fetch');
    var colors = require('colors');

    const Workspace = require('../host/workspace.js');
    const Chrome = require('../browser/chrome.js');


    class TestSuite {

        #workspace;
        #workspacePath = __dirname + '/../../../../';
        
        #testCasesPath;
        #testCases = [];

        #browser;
        #url;


        constructor(testCasesPath) {
            if(path.isAbsolute(testCasesPath)) {
                this.#testCasesPath = testCasesPath;
            } else {
                this.#testCasesPath = __dirname + '/' + testCasesPath;
            }
            this.#setTestCases(this.#testCasesPath);
        }
    

        getTestCasesPath() {
            return this.#testCasesPath;
        }

        #setTestCases(testCasesPath) {
            if (fs.lstatSync(testCasesPath).isDirectory()) {
                let files = fs.readdirSync(testCasesPath);
                files.forEach(file => {
                    if (fs.lstatSync(testCasesPath + '/' + file).isDirectory()) {
                        this.#setTestCases(testCasesPath + '/' + file);
                    }
                    else {
                        if (path.extname(file) == '.mjs') {
                            this.#testCases.push(path.relative(this.#testCasesPath, testCasesPath + '/' + path.parse(file).name));
                        }
                    }
                })
            }
        }

        #filterTestCases(filters) {
            let ans = [];
            if (filters.length == 0) {
                ans = this.#testCases;
            } else {
                filters.forEach(filter => {
                    let testCase = filter;
                    if (testCase.includes('test_cases/')) {
                        testCase = path.resolve(testCase).split('test_cases/')[1];
                    }
                    if (testCase.endsWith('.mjs')) {
                        testCase = testCase.slice(0, -path.extname(testCase).length);
                    }
                    if (this.#testCases.includes(testCase)) {
                        if (!ans.includes(testCase)) {
                            ans.push(testCase);
                        }
                    }
                });
            }
            return ans;
        }

        getTestCases() {
            return this.#testCases;
        }

        async #isUrlExist(url) {
            const response = await fetch(url, {
                method: 'HEAD'
            });
            if (response.status == 200) {
                return true;
            }
            return false;
        }

        async #setUrl(url) {
            try {
                let vizzuMinJs = 'vizzu.min.js';
                let vizzuJs = 'vizzu.js';

                if (url.endsWith(vizzuMinJs)) {
                    url = url.substring(0, url.length - vizzuMinJs.length);
                }
                if (url.endsWith(vizzuJs)) {
                    url = url.substring(0, url.length - vizzuJs.length);
                }
                if (url.endsWith('/')) {
                    url = url.substring(0, url.length - 1);
                }

                if (url.includes(remoteStableBucket)) {
                    url = 'https://' + remoteStableBucket + '/lib';
                } else if (url.includes(remoteLatestBucket)) {
                    url = 'https://' + remoteLatestBucket + '/lib-' + url.split('/lib-')[1].substring(0,7);
                }

                if (url.startsWith('https://')) {
                    if (await this.#isUrlExist(url + '/' + vizzuMinJs)) {
                        url = url + '/' + vizzuMinJs;
                    } else if (await this.#isUrlExist(url + '/' + vizzuJs)) {
                        url = url + '/' + vizzuJs;
                    } else {
                        throw new Error('ENOENT: ' + url + '/' + vizzuMinJs + '|' + url + '/' + vizzuJs);
                    }
                } else {
                    if (fs.existsSync(this.#workspacePath + url + '/' + vizzuMinJs)) {
                        url = url + '/' + vizzuMinJs;
                    } else if (fs.existsSync(this.#workspacePath + url + '/' + vizzuJs)) {
                        url = url + '/' + vizzuJs;
                    } else {
                        throw new Error('ENOENT: ' + path.resolve(this.#workspacePath + url + '/' + vizzuMinJs) + '|' + path.resolve(this.#workspacePath + url + '/' + vizzuJs));
                    }
                }
                this.#url = url;
                console.log('[ ' + 'URL'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + this.#url + ' ]');
            } catch (err) {
                console.error(('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ vizzUrl is incorrect ]').error);
                throw err;
            }
        }

        async runTestSuite() {
            try {
                await this.#setUrl(argv.vizzuUrl);
                let testCases = this.#filterTestCases(argv._);
                if (testCases.length > 0) {
                    this.#startTestSuite();
                    for (let i = 0; i < testCases.length; i++) {
                        await this.#runTestCase(testCases[i]);
                    }
                }
            } catch (err) {
                throw err;
            } finally {
                this.#finishTestSuite();
            }
        }

        #startTestSuite() {
            this.#workspace = new Workspace(this.#workspacePath);
            this.#workspace.openWorkspace();
            console.log('[ HOSTING ]' + ' ' + '[ ' + 'http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + ' ]');
            this.#browser = new Chrome();
            this.#browser.openBrowser(!argv.disableHeadlessBrowser);
        }

        #finishTestSuite() {
            let errs = [];
            try {
                if(typeof this.#browser !== 'undefined') {
                    this.#browser.closeBrowser();
                }
            } catch (err) {
                errs.push(err);
            }
            try {
                if(typeof this.#workspace !== 'undefined') {
                    this.#workspace.closeWorkspace();
                }
            } catch (err) {
                errs.push(err);
            }
            if (errs.length > 1) {
                throw new AggregateError(errs);
            } else if (errs.length == 1) {
                throw errs[0];
            }
        }

        async #runTestCase(testCase) {
            let testCaseData = await this.#runTestCaseClient(testCase, this.#url);
            if (testCaseData.result == 'DONE') {
                console.log('[ ' + testCaseData.result.padEnd(padLength, ' ') + ' ] ' + testCase);
            } else {
                let errParts = testCaseData.description.split('http://127.0.0.1:' + + String(this.#workspace.getWorkspacePort())).join(path.resolve(this.#workspacePath)).split('\n');
                console.error(('[ ' + testCaseData.result.padEnd(padLength, ' ') + ' ] ' + '[ ' + errParts[0] + ' ] ').error + testCase);
                if (errParts.length > 1) {
                    errParts.slice(1).forEach(item => {
                        console.error(''.padEnd(padLength + 7, ' ') + item);
                    });
                }
            }
        }

        async #runTestCaseClient(testCase, vizzuUrl) {
            await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort())
                + '/test/integration/modules/videorecorder/index.html'
                + '?testCase=' + testCase
                + '&vizzuUrl=' + vizzuUrl);
            const now = Date.now();
            const timeout = defaultTestCaseTimeout;
            while (true) {
                if (Date.now() > now + timeout) {
                    return { result: 'ERROR', description: 'timeout' };
                }
                let testCaseData= await this.#browser.executeScript('if (window.hasOwnProperty("data")) { return data } else { return \'undefined\' }');
                if (testCaseData != 'undefined') {
                    return testCaseData;
                }
                await new Promise(resolve => setTimeout(resolve, 50));
            }
        }
    }


    colors.setTheme({
        warn: 'yellow',
        error: 'red',
        success: 'green'
    });

    var argv = yargs
        .usage('Usage: $0 [test_cases] [options]')

        .example('$0', 'Run all tests in the test_cases folder')
        .example('$0 test_cases/*', 'Run all tests in the test_cases folder')
        .example('$0 test_cases/example.mjs', 'Run example.mjs')
        .example('$0 test_cases/exampl?.mjs', 'Run example.mjs')
        
        .help('h')
        .alias('h', 'help')
        .version('0.0.1')
        .alias('v', 'version')
        .boolean('b')
        .alias('b', 'disableHeadlessBrowser')
        .default('b', false)
        .describe('b', 'Disable to use headless browser')
        .alias('u', 'vizzuUrl')
        .describe('u', 'Change vizzu.min.js url')
        .nargs('u', 1)
        .default('u', '/example/lib')
        .argv;

    let test = new TestSuite(__dirname + '/../../test_cases');
    test.runTestSuite();
} catch (err) {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    console.error('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ] ' + errMsg);
}
