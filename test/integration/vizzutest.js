const { resolve } = require('path');

const remoteLatestBucket = 'vizzu-lib-main-sha.storage.googleapis.com';
const remoteStableBucket = 'vizzu-lib-main.storage.googleapis.com';
const defaultAnimStep = '20%';
const defaultTestCaseTimeout = 60000;
const padLength = 7;


try {

    const AggregateError = import('aggregate-error');

    const fs = require('fs');
    const path = require('path');
    const yargs = require('yargs');
    const fetch = require('node-fetch');
    var colors = require('colors');

    const Workspace = require('./modules/host/workspace.js');
    const Chrome = require('./modules/browser/chrome.js');


    class TestSuite {

        #workspace;
        #workspacePath = __dirname + '/../../';
        
        #testCasesPath;
        #testCases = [];
        #testCasesData = {};
        #testCasesDataPath;

        #testSuiteResults = { 'PASSED': [], 'WARNING': [], 'FAILED': [] };

        #browser;
        #browserKey = 'chrome';
        #browserMode = 'headless';
        #url;

        #osKey = 'ubuntu_focal';


        constructor(testCasesPath) {
            if(path.isAbsolute(testCasesPath)) {
                this.#testCasesPath = testCasesPath;
            } else {
                this.#testCasesPath = __dirname + '/' + testCasesPath;
            }
            this.#setTestCases(this.#testCasesPath);
            this.#setUrl(argv.vizzuUrl);
            if (argv.disableHeadlessBrowser) {
                this.#browserMode = 'gui';
            }
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
            this.#testCasesDataPath = this.#testCasesPath + '/../test_cases.json';
            if (fs.existsSync(this.#testCasesDataPath)) {
                this.#testCasesData = JSON.parse(fs.readFileSync(this.#testCasesDataPath));
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
                console.log('[ ' + 'URL'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + this.#url + '/vizzu.js ]');
            } catch (err) {
                console.error(('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ vizzUrl is incorrect ]').error);
                throw err;
            }
        }

        async runTestSuite() {
            try {
                let testCases = this.#filterTestCases(argv._)
                if (testCases.length > 0) {
                    this.#startTestSuite();
                    for (let i = 0; i < testCases.length; i++) {
                        await this.#runTestCase(testCases[i]);
                    }
                    if (argv.reportLevel != 'DISABLED') {
                        this.#createJson(__dirname + '/test_report/', testCases);
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
                    console.error(''.padEnd(padLength + 5, ' ') + testCase);
                });
            } else {
                console.log('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length);
            }
        }

        async #runTestCase(testCase) {
            let testCaseResultPath = __dirname + '/test_report/' + testCase;
            let testCaseData = await this.#runTestCaseClient(testCase, this.#url);
            let testCaseResultObject = this.#getTestCaseResult(testCaseData, testCase);
            let testCaseResult = testCaseResultObject.testCaseResult;
            fs.rmSync(testCaseResultPath, { recursive: true, force: true });

            let createReport = false;
            if (testCaseResult == 'PASSED') {
                console.log(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ').success + testCase);
                this.#testSuiteResults.PASSED.push(testCase);
                if (argv.reportLevel == 'INFO') {
                    createReport = true;
                }
            } else if (testCaseResult == 'WARNING') {
                console.warn(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ' + '[ ' + testCaseResultObject.testCaseReultDescription + ' ] ').warn + testCase);
                this.#testSuiteResults.WARNING.push(testCase);
                if (argv.reportLevel == 'INFO' || argv.reportLevel == 'WARN') {
                    createReport = true;
                }
            } else {
                let errParts = testCaseResultObject.testCaseReultDescription.split('http://127.0.0.1:' + + String(this.#workspace.getWorkspacePort())).join(path.resolve(this.#workspacePath)).split('\n');
                console.error(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ' + '[ ' + errParts[0] + ' ] ').error + testCase);
                if (errParts.length > 1) {
                    errParts.slice(1).forEach(item => {
                        console.error(''.padEnd(padLength + 7, ' ') + item);
                    });
                }
                this.#testSuiteResults.FAILED.push(testCase);
                if (testCaseResult == 'FAILED') {
                    if (argv.reportLevel == 'INFO' || argv.reportLevel == 'WARN' || argv.reportLevel == 'ERROR') {
                        createReport = true;
                    }
                }
            }

            if (createReport) {
                fs.mkdirSync(testCaseResultPath, { recursive: true });
                this.#createImages(testCaseResultPath, testCase, testCaseData, false);
                if (testCaseResult == 'FAILED') {
                    let sha;
                    if (testCaseResult == 'FAILED') {
                        try {
                            let shaUrl = await fetch('https://' + remoteStableBucket + '/lib/sha.txt');
                            sha = await shaUrl.text();
                            let vizzuUrl = 'https://' + remoteLatestBucket + '/lib-' + sha;
                            let testCaseRefData = await this.#runTestCaseClient(testCase, vizzuUrl);
                            let diff = false;
                            for (let i = 0; i < testCaseData.hashes.length; i++) {
                                for (let j = 0; j < testCaseData.hashes[i].length; j++) {
                                    if (testCaseData.hashes[i][j] != testCaseRefData.hashes[i][j]) {
                                        console.log(''.padEnd(padLength + 5, ' ') + '[ ' + 'step: ' + i + '. - seek: ' + testCaseData.seeks[i][j] + ' - hash: ' + testCaseData.hashes[i][j].substring(0,7) + ' ' + '(ref: ' + testCaseRefData.hashes[i][j].substring(0,7) + ')' + ' ]');
                                        diff = true
                                    }
                                }
                            }
                            if (!diff) {
                                console.warn(''.padEnd(padLength + 5, ' ') + '[ the currently counted hashes are the same, the difference is probably caused by the environment ]');
                            }
                            this.#createImages(testCaseResultPath, testCase, testCaseRefData, true);
                        } catch (err) {
                            let libSha = '';
                            if(typeof sha !== 'undefined') {
                                libSha = ' with lib-' + sha.trim();
                            }
                            console.warn(('[ ' + 'WARNING'.padEnd(padLength, ' ') + ' ] ' + '[ ' + 'can not create ref' + libSha + ' (' + err.toString() + ') ] ').warn + testCase);
                        }
                    }
                }
            }
        }

        async #runTestCaseClient(testCase, vizzuUrl) {
            let animstep = defaultAnimStep;
            if (testCase in this.#testCasesData) {
                if ('animstep' in this.#testCasesData[testCase]) {
                    animstep = this.#testCasesData[testCase]['animstep'].replace('%', '');
                }
            }
            await this.#browser.getUrl('http://127.0.0.1:' + String(this.#workspace.getWorkspacePort())
                + '/test/integration/modules/client/index.html'
                + '?testCase=' + testCase
                + '&vizzuUrl=' + vizzuUrl
                + '&animstep=' + animstep);
            const now = Date.now();
            const timeout = defaultTestCaseTimeout;
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

        #getTestCaseRefHash(testCase, hashKey) {
            try {
                let hash = this.#testCasesData[testCase]['refs'][hashKey];
                if (hash == '' || typeof hash === 'undefined') {
                    throw new TypeError('ref hash does not exist');
                } else {
                    return hash;
                }
            } catch (err) {
                if (err instanceof TypeError) {
                    return undefined;
                }
                throw err;
            }
        }

        #getTestCaseResult(testCaseData, testCase) {
            if (testCaseData.result != 'FINISHED') {
                return { testCaseResult: testCaseData.result, testCaseReultDescription: testCaseData.description };
            } else {
                if (!(testCase in this.#testCasesData)) {
                    this.#testCasesData[testCase] = { refs: {} };
                } else {
                    if ('animstep' in this.#testCasesData[testCase]) {
                        if (this.#testCasesData[testCase]['animstep'].replace('%', '') == defaultAnimStep.replace('%', '')) {
                            delete this.#testCasesData[testCase]['animstep'];
                        }
                    }
                }
                let defaultHashKey = 'ubuntu_focal_chrome_headless';
                let hashKey = this.#osKey + '_' + this.#browserKey + '_' + this.#browserMode;
                let hashRef = this.#getTestCaseRefHash(testCase, hashKey);
                if (typeof hashRef === 'undefined') {
                    hashKey = defaultHashKey;
                    hashRef = this.#getTestCaseRefHash(testCase, hashKey);
                }
                this.#testCasesData[testCase]['refs'][hashKey] = testCaseData.hash.substring(0,7);
                if (typeof hashRef === 'undefined') {
                    return { testCaseResult: 'WARNING', testCaseReultDescription: 'ref hash does not exist' };
                }
                if (hashRef != testCaseData.hash.substring(0,7)) {
                    return { testCaseResult: 'FAILED', testCaseReultDescription: 'hash: ' + testCaseData.hash.substring(0,7) + ' ' + '(ref: ' + hashRef.substring(0,7) + ')' };
                }
            }
            return { testCaseResult: 'PASSED' };
        }

        #createImages(testCaseResultPath, testCase, testCaseData, isRef) {
            let fileAdd = ''
            if (isRef) {
                fileAdd = '-ref'
            }
            fs.mkdirSync(testCaseResultPath, { recursive: true });
            for (let i = 0; i < testCaseData.seeks.length; i++) {
                for (let j = 0; j < testCaseData.seeks[i].length; j++) {
                    let seek = (testCaseData.seeks[i][j].replace('%', '')).split('.');
                    if (seek.length == 1) {
                        seek.push('0');
                    }
                    fs.writeFile(testCaseResultPath + '/' + path.basename(testCaseResultPath) + '_' + i.toString().padStart(3, '0') + '_' + seek[0].padStart(3, '0') + '.' + seek[1].padEnd(3, '0') + '%' + fileAdd + '.png', testCaseData.images[i][j].substring(22), 'base64', err => {
                        if (err) {
                            throw err;
                        }
                    });
                }
            }
        }

        #createJson(testSuiteResultPath, testCases) {
            fs.mkdirSync(testSuiteResultPath, { recursive: true });
            let testCasesData = {};
            testCases.forEach(testCase => {
                testCasesData[testCase] = this.#testCasesData[testCase];
            });
            testCasesData = JSON.stringify(testCasesData, null, 4);
            fs.writeFile(testSuiteResultPath + 'test_cases.json', testCasesData, (err) => {
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
        .alias('r', 'reportLevel')
        .choices('r', ['INFO', 'WARN', 'ERROR', 'DISABLED'])
        .default('r', 'INFO')
        .describe('r', 'Set report level')
        .alias('u', 'vizzuUrl')
        .describe('u', 'Change vizzu.js url')
        .nargs('u', 1)
        .default('u', '/example/lib')
        .argv;

    let test = new TestSuite(__dirname + '/test_cases');
    test.runTestSuite();
} catch (err) {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    console.error('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ] ' + errMsg);
}
