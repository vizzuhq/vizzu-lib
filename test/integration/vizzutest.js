const AggregateError = import('aggregate-error');

const fs = require('fs');
const path = require('path');
const yargs = require('yargs');
const fetch = require('node-fetch');
var colors = require('colors');

const Workspace = require('./modules/host/workspace.js');
const Chrome = require('./modules/browser/chrome.js');

const remoteLatestBucket = 'vizzu-lib-main-sha.storage.googleapis.com';
const remoteStableBucket = 'vizzu-lib-main.storage.googleapis.com';


class TestSuite {

    #workspace;
    
    #testCasesPath;
    #testCases = [];

    #testSuiteResults = { 'PASSED': [], 'WARNING': [], 'FAILED': [] };
    #padLength = 0;

    #browser;
    #url;


    constructor(testCasesPath) {
        if(path.isAbsolute(testCasesPath)) {
            this.#testCasesPath = testCasesPath;
        } else {
            this.#testCasesPath = __dirname + '/' + testCasesPath;
        }
        this.#setTestCases(this.#testCasesPath);
        this.#setPadLength();
        this.#setUrl(argv.vizzuUrl);
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

    #setUrl(url) {
        try {
            if (url.includes(remoteStableBucket)) {
                this.#url = 'https://' + remoteStableBucket + '/lib';
            } else if (url.includes(remoteLatestBucket)) {
                this.#url = 'https://' + remoteLatestBucket + '/lib-' + url.split('/lib-')[1].substring(0,7);
            } else {
                let vizzuJs = 'vizzu.js'
                if (url.endsWith(vizzuJs)) {
                    url = url.substring(0, url.length - vizzuJs.length);
                }
                if (url.endsWith('/')) {
                    url = url.substring(0, url.length - 1);
                }
                this.#url = url;
            }
        } catch (err) {
            console.error(('[ ' + 'ERROR'.padEnd(this.#padLength, ' ') + ' ]' + ' ' + '[ vizzUrl is incorrect ]').error);
            throw err;
        }
    }

    #setPadLength() {
        Object.keys(this.#testSuiteResults).forEach(key => {
            if (key.length > this.#padLength) {
                this.#padLength = key.length;
            }
        })
    }

    async runTestSuite() {
        try {
            let testCases = this.#filterTestCases(argv._)
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
        this.#workspace = new Workspace(__dirname + '/../../');
        this.#workspace.openWorkspace();
        console.log('[ HOSTING ]' + ' ' + '[ ' + 'http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + ' ]');
        this.#browser = new Chrome();
        this.#browser.openBrowser(!argv.disableHeadlessBrowser);
    }

    #finishTestSuite() {
        let errs = [];
        try {
            this.#createTestSuiteResults();
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

    #createTestSuiteResults() {
        console.log('\n' + 'all tests:'.padEnd(15, ' ') + this.#testCases.length);
        const sum = this.#testSuiteResults.PASSED.length + this.#testSuiteResults.WARNING.length + this.#testSuiteResults.FAILED.length;
        console.log('tests run:'.padEnd(15, ' ') + sum);
        console.log(('tests passed:'.padEnd(15, ' ') + this.#testSuiteResults.PASSED.length).success);
        if (this.#testSuiteResults.WARNING.length != 0) {
            console.warn(('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length).warn);
        } else {
            console.log('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length);
        }
        if (this.#testSuiteResults.FAILED.length != 0) {
            console.error(('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length).error);
            process.exitCode = 1;
            this.#testSuiteResults.FAILED.forEach(testCase => {
                console.error(''.padEnd(this.#padLength + 5, ' ') + testCase);
            });
        } else {
            console.log('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length);
        }
    }

    async #runTestCase(testCase) {
        let testSuiteResultPath = __dirname + '/test_report/' + testCase;
        let testCaseData = await this.#runTestCaseClient(testCase, this.#url);
        let testCaseResultObject = this.#getTestCaseResult(testCaseData);
        let testCaseResult = testCaseResultObject.testCaseResult;
        fs.rmSync(testSuiteResultPath, { recursive: true, force: true });

        if (testCaseResult == 'PASSED') {
            console.log(('[ ' + testCaseResult.padEnd(this.#padLength, ' ') + ' ] ').success + testCase);
            this.#testSuiteResults.PASSED.push(testCase);
        } else if (testCaseResult == 'WARNING') {
            console.warn(('[ ' + testCaseResult.padEnd(this.#padLength, ' ') + ' ] ' + '[ ' + testCaseResultObject.testCaseReultDescription + ' ] ').warn + testCase);
            this.#testSuiteResults.WARNING.push(testCase);
        } else {
            console.error(('[ ' + testCaseResult.padEnd(this.#padLength, ' ') + ' ] ' + '[ ' + testCaseResultObject.testCaseReultDescription + ' ] ').error + testCase);
            this.#testSuiteResults.FAILED.push(testCase);
        }

        if (!argv.disableReport) {
            if (testCaseResult == 'FAILED' || testCaseResult == 'WARNING') {
                fs.mkdirSync(testSuiteResultPath, { recursive: true });
                this.#createTestCaseReport(testSuiteResultPath, testCase, testCaseData, false);
                let sha;
                if (testCaseResult == 'FAILED') {
                    try {
                        let shaUrl = await fetch('https://' + remoteStableBucket + '/lib/sha.txt');
                        sha = await shaUrl.text();
                        let vizzuUrl = 'https://' + remoteLatestBucket + '/lib-' + sha;
                        let refData = await this.#runTestCaseClient(testCase, vizzuUrl);
                        this.#createTestCaseReport(testSuiteResultPath, testCase, refData, true);
                    } catch (err) {
                        let libSha = '';
                        if(typeof sha !== 'undefined') {
                            libSha = ' with lib-' + sha.trim();
                        }
                        console.warn(('[ ' + 'WARNING'.padEnd(this.#padLength, ' ') + ' ] ' + '[ ' + 'can not create ref' + libSha + ' (' + err.toString() + ') ] ').warn + testCase);
                    }
                }
            }
        }
    }

    async #runTestCaseClient(testCase, vizzuUrl) {
        await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort())
            + '/test/integration/modules/client/index.html'
            + '?testCase=' + testCase
            + '&vizzuUrl=' + vizzuUrl);
            const now = Date.now();
            const timeout = 60000;
            while (true) {
                if (Date.now() > now + timeout) {
                    return { result: 'ERROR', description: 'timeout' };
                }
                let testCaseData= await this.#browser.executeScript('if (window.hasOwnProperty("testData")) { return testData } else { return \'undefined\' }');
                if (testCaseData != 'undefined') {
                    return testCaseData;
                }
                await new Promise(resolve => setTimeout(resolve, 50));
            }
    }

    #getTestCaseResult(testCaseData) {
        if (testCaseData.result != 'FINISHED') {
            return { testCaseResult: testCaseData.result, testCaseReultDescription: testCaseData.description };
        } else {
            for (let i = 0; i < testCaseData.seeks.length; i++) {
                for (let j = 0; j < testCaseData.seeks[i].length; j++) {
                    if (testCaseData.references[i][j] == '') {
                        return { testCaseResult: 'WARNING', testCaseReultDescription: 'ref hash does not exist' };
                    }
                    if (testCaseData.hashes[i][j] != testCaseData.references[i][j]) {
                        return { testCaseResult: 'FAILED', testCaseReultDescription: 'hash: ' + testCaseData.hashes[i][j] + ' ' + '(ref: ' + testCaseData.references[i][j] + ')' };
                    }
                }
            }
        }
        return { testCaseResult: 'PASSED' };
    }

    #createTestCaseReport(testSuiteResultPath, testCase, testCaseData, isRef) {
        let fileAdd = ''
        if (isRef) {
            fileAdd = '-ref'
        }
        let hashList = [];
        fs.mkdirSync(testSuiteResultPath, { recursive: true });
        for (let i = 0; i < testCaseData.seeks.length; i++) {
            hashList[i] = {};
            for (let j = 0; j < testCaseData.seeks[i].length; j++) {
                hashList[i][testCaseData.seeks[i][j]] = testCaseData.hashes[i][j];
                if (isRef) {
                    hashList[i][testCaseData.seeks[i][j]] = testCaseData.references[i][j];
                }
                fs.writeFile(testSuiteResultPath + '/' + path.basename(testSuiteResultPath) + i + '_' + testCaseData.seeks[i][j] + fileAdd + ".png", testCaseData.images[i][j].substring(22), 'base64', err => {
                    if (err) {
                        throw err;
                    }
                });
            }
        }
        hashList = JSON.stringify(hashList, null, 4);
        fs.writeFile(testSuiteResultPath + '/' + path.basename(testSuiteResultPath) + fileAdd + '.json', hashList, (err) => {
            if (err) {
                throw err;
            }
        });
    }
}


colors.setTheme({
    warn: 'yellow',
    error: 'red',
    success: 'green'
});


try {
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
        .boolean('r')
        .alias('r', 'disableReport')
        .default('r', false)
        .describe('r', 'Disable to create detailed report')
        .alias('u', 'vizzuUrl')
        .describe('u', 'Change vizzu.js url')
        .nargs('u', 1)
        .default('u', '/example/lib')
        .argv;

    let test = new TestSuite(__dirname + '/test_cases');
    test.runTestSuite(argv);
} catch (err) {
    console.error(err.error);
    process.exitCode = 1;
}  
