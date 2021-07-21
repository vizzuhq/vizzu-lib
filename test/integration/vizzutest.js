const AggregateError = import('aggregate-error');

const fs = require('fs');
const path = require('path');
const yargs = require('yargs');
const fetch = require('node-fetch');

const Workspace = require('./modules/host/workspace.js');
const Chrome = require('./modules/browser/chrome.js');


class TestSuite {

    #workspacePath = __dirname + '/../..';
    #workspace;
    
    #testCasesPath;
    #testCases = [];

    #testResults = { 'passed': [], 'failed': [] };

    #browser;


    constructor(testCasesPath) {
        if(path.isAbsolute(testCasesPath)) {
            this.#testCasesPath = testCasesPath;
        } else {
            this.#testCasesPath = __dirname + '/' + testCasesPath;
        }
        this.#setTestCases();
    }
  

    getTestCasesPath() {
        return this.#testCasesPath;
    }

    getTestCases() {
        return this.#testCases;
    }

    async runTestSuite() {
        try {
            let testCases = this.#filterTestCases(argv._)
            console.log('Selected Test Cases: ' + testCases);
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

    #setTestCases() {
        let files = fs.readdirSync(this.#testCasesPath);
        files.forEach(file => {
            if (path.extname(file) == '.mjs') {
                this.#testCases.push(path.parse(file).name);
            }
        })
        console.log('Test Cases: ' + this.#testCases);
    }

    #filterTestCases(filters) {
        let ans = [];
        if (filters.length == 0) {
            ans = this.#testCases;
        } else {
            filters.forEach(filter => {
                let testCase = path.parse(filter).base;
                if (this.#testCases.includes(testCase)) {
                    if (path.relative(filter, this.#testCasesPath + '/' + testCase) == '') {
                        if (!ans.includes(testCase)) {
                            ans.push(testCase);
                        }
                    }
                }
            });
        }
        return ans;
    }

    async #runVizzu(testCase, vizzuUrl) {
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort())
            + '/test/integration/modules/client/index.html'
            + '?testCase=' + testCase
            + '&vizzuUrl=' + vizzuUrl);
            const now = Date.now();
            const timeout = 60000;
            while (true) {
                if (Date.now() > now + timeout) {
                    return { result: 'TIMEOUT' };
                }
                let testResult= await this.#browser.executeScript('if (window.hasOwnProperty("results")) { return results } else { return \'undefined\' }');
                if (testResult != 'undefined') {
                    return testResult;
                }
                await new Promise(resolve => setTimeout(resolve, 50));
            }
    }

    async #runTestCase(testCase) {
        let testResultPath = __dirname + '/testReport/' + testCase;
        fs.rmdirSync(testResultPath, { recursive: true });
        let testResult = await this.#runVizzu(testCase, argv.vizzuUrl);
        if (testResult.result == 'PASSED') {
            console.log(testCase + ' : ' + testResult.result);
            this.#testResults.passed.push(testCase);
        } else {
            console.error(testCase + ' : ' + testResult.result);
            this.#testResults.failed.push(testCase);
        }
        if (testResult.result == 'FAILED' && !argv.disableReport) {
            fs.mkdirSync(testResultPath, { recursive: true });
            let hashList = [];
            for (let i = 0; i < testResult.seeks.length; i++) {
                hashList[i] = {};
                for (let j = 0; j < testResult.seeks[i].length; j++) {
                    hashList[i][testResult.seeks[i][j]] = testResult.hashes[i][j];
                    fs.writeFile(testResultPath + '/' + testCase + '_' + i + '_' + testResult.seeks[i][j] + ".png", testResult.images[i][j].substring(22), 'base64', err => {
                        if (err) {
                            throw err;
                        }
                    });
                }
            }
            hashList = JSON.stringify(hashList, null, 4);
            fs.writeFile(testResultPath + '/' + testCase + '.json', hashList, (err) => {
                if (err) {
                    throw err;
                }
            });
            try {
                let hashPath = this.#testCasesPath + '/' + testCase + '.json';
                let hashObject = JSON.parse(fs.readFileSync(hashPath, 'utf8'));
                let isRefExist = true;
                loop1:
                    for (let i = 0; i < hashObject.length; i++) {
                        let keys = Object.keys(hashObject[i]);
                        loop2:
                            for (let j = 0; i < keys.length; i++) {
                                if (hashObject[i][keys[j]] == '') {
                                    isRefExist = false;
                                    break loop1;
                                }
                            }
                    }
                if (isRefExist) {
                    let sha = await fetch('https://vizzu-lib-main.storage.googleapis.com/lib/sha');
                    let vizzuUrl = 'https://vizzu-lib-main-sha.storage.googleapis.com/lib-' + await sha.text();
                    let refResult = await this.#runVizzu(testCase, vizzuUrl);
                    for (let i = 0; i < refResult.seeks.length; i++) {
                        for (let j = 0; j < refResult.seeks[i].length; j++) {
                            fs.writeFile(testResultPath + '/' + testCase + '_' + i + '_' + refResult.seeks[i][j] + "-ref.png", refResult.images[i][j].substring(22), 'base64', err => {
                                if (err) {
                                    throw err;
                                }
                            });
                        }
                    }
                    fs.copyFileSync(hashPath, testResultPath + '/' + testCase + '-ref.json');
                }
            } catch (err) {
                console.error(err);
            }
        }
    }

    #startTestSuite() {
        this.#workspace = new Workspace(__dirname + '/../../');
        this.#workspace.openWorkspace();
        console.log('Listening at http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()));
        this.#browser = new Chrome();
        this.#browser.openBrowser(!argv.disableHeadlessBrowser);
    }

    #logTestSuiteResults() {
        const sum = this.#testResults.passed.length + this.#testResults.failed.length
        if (this.#testResults.passed.length != sum) {
            console.error('PASSED : ' + sum + '/' + this.#testResults.passed.length + 
                    ', FAILED : ' + sum + '/' + this.#testResults.failed.length)
            process.exitCode = 1;
        } else {
            console.log('PASSED : ' + sum + '/' + this.#testResults.passed.length + 
                    ', FAILED : ' + sum + '/' + this.#testResults.failed.length)
        }
    }

    #finishTestSuite() {
        let errs = [];
        try {
            this.#logTestSuiteResults();
        } catch (err) {
            errs.push(err);
        }
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
}


try {
    var argv = yargs
        .usage('Usage: $0 [testCases] [options]')

        .example('$0', 'Run all tests in the testCases folder')
        .example('$0 testCases/*', 'Run all tests in the testCases folder')
        .example('$0 testCases/example.mjs', 'Run example.mjs')
        .example('$0 testCases/exampl?.js', 'Run example.mjs')
        
        .help('h')
        .alias('h', 'help')
        .version('0.0.1')
        .alias('v', 'version')
        .boolean('b')
        .alias('b', 'disableHeadlessBrowser')
        .default('b', false)
        .describe('b', 'Disable to use headless browser')
        .boolean('r')
        .alias('r', 'disableReport')
        .default('r', false)
        .describe('r', 'Disable to create detailed report')
        .alias('u', 'vizzuUrl')
        .describe('u', 'Change vizzu.js url')
        .nargs('u', 1)
        .default('u', '/example/lib')
        .argv;

    let test = new TestSuite(__dirname + '/testCases');
    test.runTestSuite(argv);
} catch (err) {
    console.error(err);
    process.exitCode = 1;
}  
