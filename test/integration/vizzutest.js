const fs = require('fs');
var colors = require('colors');
const strip = require('strip-color');

const remoteLatestBucket = 'https://vizzu-lib-main-sha.storage.googleapis.com';
const remoteStableBucket = 'https://vizzu-lib-main.storage.googleapis.com';
const remoteCdn = 'https://cdn.jsdelivr.net/npm/vizzu';
const defaultAnimStep = '20%';
const defaultTestCaseTimeout = 60000;
const padLength = 7;

const date = new Date();
const timestamp = date.getFullYear() + ("0" + (date.getMonth() + 1)).slice(-2) + ("0" + date.getDate()).slice(-2) + '_' + date.getHours() + date.getMinutes() + date.getSeconds();

colors.setTheme({
    warn: 'yellow',
    error: 'red',
    success: 'green'
});

const console = require('console');
const { Console } = console;
var fileConsole;

const log = (message, reportLevel='DISABLED') => {
    console.log(message);
    if (reportLevel !== 'DISABLED') {
        if (fileConsole === undefined) {
            fs.mkdirSync(__dirname + '/test_report/logs', { recursive: true });
            const out = fs.createWriteStream(__dirname + '/test_report/logs/vizzutest_' + timestamp + '.log');
            fileConsole = new console.Console(out, out);
        }
        fileConsole.log(strip(message));
    }
};


try {
    const AggregateError = import('aggregate-error');

    const path = require('path');
    const yargs = require('yargs');
    const fetch = require('node-fetch');
    const PNG = require('pngjs').PNG;
    const pixelmatch = require('pixelmatch');

    const Workspace = require('./modules/host/workspace.js');
    const Chrome = require('./modules/browser/chrome.js');


    class TestSuite {

        #workspace;
        #workspacePath = __dirname + '/../../';
        
        #testCasesPath;
        #testCases = [];
        #testCasesRun = [];
        #testCasesData = {};
        #testCasesDataPath;

        #testSuiteResults = { 'PASSED': [], 'WARNING': [], 'FAILED': [] };

        #browser;
        #url;
        #refurl;


        constructor(testCasesPath) {
            log('[ ' + 'TIME'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + timestamp + ' ]', argv.reportLevel);
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
            this.#testCasesDataPath = this.#workspacePath + argv.cases;
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
                let vizzuJs = 'vizzu.js';
                let vizzuMinJs = 'vizzu.min.js';
                let vizzuTypeForced;

                if (url.endsWith(vizzuMinJs)) {
                    url = url.substring(0, url.length - vizzuMinJs.length);
                    vizzuTypeForced = vizzuMinJs;
                } else if (url.endsWith(vizzuJs)) {
                    url = url.substring(0, url.length - vizzuJs.length);
                    vizzuTypeForced = vizzuJs
                }

                if (url.endsWith('/')) {
                    url = url.substring(0, url.length - 1);
                }

                if (url.toLowerCase() === 'head') {
                    url = remoteStableBucket + '/lib';
                } else if (/^[A-Za-z0-9]+$/.test(url) && url.length === 7) {
                    url = remoteLatestBucket + '/lib-' + url;
                } else if (/^(\d+\.)?(\d+\.)?(\*|\d+)$/.test(url)) {
                    url = remoteCdn + '@' + url + '/dist';
                    vizzuTypeForced = vizzuMinJs;
                }

                if (url.startsWith('https://')) {
                    if (vizzuTypeForced) {
                        if (await this.#isUrlExist(url + '/' + vizzuTypeForced)) {
                            url = url + '/' + vizzuTypeForced;
                        } else {
                            throw new Error('ENOENT: ' + url + '/' + vizzuTypeForced);
                        }
                    } else {
                        if (await this.#isUrlExist(url + '/' + vizzuJs)) {
                            url = url + '/' + vizzuJs;
                        } else if (await this.#isUrlExist(url + '/' + vizzuMinJs)) {
                            url = url + '/' + vizzuMinJs;
                        } else {
                            throw new Error('ENOENT: ' + url + '/' + vizzuJs + '|' + url + '/' + vizzuMinJs);
                        }
                    }
                } else {
                    if (vizzuTypeForced) {
                        if (fs.existsSync(this.#workspacePath + url + '/' + vizzuTypeForced)) {
                            url = url + '/' + vizzuTypeForced;
                        } else {
                            throw new Error('ENOENT: ' + url + '/' + vizzuTypeForced);
                        }
                    } else {
                        if (fs.existsSync(this.#workspacePath + url + '/' + vizzuJs)) {
                            url = url + '/' + vizzuJs;
                        } else if (fs.existsSync(this.#workspacePath + url + '/' + vizzuMinJs)) {
                            url = url + '/' + vizzuMinJs;
                        } else {
                            throw new Error('ENOENT: ' + path.resolve(this.#workspacePath + url + '/' + vizzuJs) + '|' + path.resolve(this.#workspacePath + url + '/' + vizzuMinJs));
                        }
                    }
                }
                this.#url = url;
                log('[ ' + 'URL'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + this.#url + ' ]', argv.reportLevel);
            } catch (err) {
                log(('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ vizzUrl is incorrect ]').error, argv.reportLevel);
                throw err;
            }
        }

        async runTestSuite() {
            try {
                await this.#setUrl(argv.vizzuUrl.toString());
                this.#testCasesRun = this.#filterTestCases(argv._);
                if (this.#testCasesRun.length > 0) {
                    this.#startTestSuite();
                    log('[ ' + 'TESTS'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + this.#testCasesRun.length + ' / ' + this.#testCases.length + ' ]', argv.reportLevel);
                    for (let i = 0; i < this.#testCasesRun.length; i++) {
                        await this.#runTestCase(i);
                    }
                    if (argv.reportLevel != 'DISABLED' && argv.t != 'DISABLED') {
                        this.#createJson(__dirname + '/test_report/results/', this.#testCasesRun);
                    }
                }
            } catch (err) {
                throw err;
            } finally {
                this.#finishTestSuite();
            }
        }

        #startTestSuite() {
            fs.rmSync(__dirname + '/test_report/results/' + 'test_cases.json', { force: true });
            this.#workspace = new Workspace(this.#workspacePath);
            this.#workspace.openWorkspace();
            log('[ ' + 'HOST'.padEnd(padLength, ' ') + ' ]' + ' ' + '[ ' + 'http://127.0.0.1:' + String(this.#workspace.getWorkspacePort()) + ' ]', argv.reportLevel);
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
                    let logPath;
                    if (argv.reportLevel !== 'DISABLED') {
                        fs.mkdirSync(__dirname + '/test_report/logs', { recursive: true });
                        logPath = __dirname + '/test_report/logs/chromedriver_' + timestamp + '.log';
                    }
                    this.#browser.closeBrowser(logPath);
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
            log('\n' + 'all tests:'.padEnd(15, ' ') + this.#testCases.length, argv.reportLevel);
            const sum = this.#testSuiteResults.PASSED.length + this.#testSuiteResults.WARNING.length + this.#testSuiteResults.FAILED.length;
            log('tests run:'.padEnd(15, ' ') + sum, argv.reportLevel);
            log(('tests passed:'.padEnd(15, ' ') + this.#testSuiteResults.PASSED.length).success, argv.reportLevel);
            if (this.#testSuiteResults.WARNING.length != 0) {
                log(('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length).warn, argv.reportLevel);
            } else {
                log('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length, argv.reportLevel);
            }
            if (this.#testSuiteResults.FAILED.length != 0) {
                log(('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length).error, argv.reportLevel);
                process.exitCode = 1;
                this.#testSuiteResults.FAILED.forEach(testCase => {
                    log(''.padEnd(padLength + 5, ' ') + testCase + ' http://127.0.0.1:8080/test/integration/manual/?version=localhost&testCase=' + testCase, argv.reportLevel);
                });
            } else {
                log('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length, argv.reportLevel);
            }
        }

        async #runTestCase(i) {
            let testCase = this.#testCasesRun[i];
            let testCaseResultPath = __dirname + '/test_report/results/' + testCase;
            let testCaseData = await this.#runTestCaseClient(testCase, this.#url);
            let testCaseResultObject = this.#getTestCaseResult(testCaseData, testCase);
            let testCaseResult = testCaseResultObject.testCaseResult;
            fs.rmSync(testCaseResultPath, { recursive: true, force: true });

            let createReport = false;
            if (testCaseResult == 'PASSED') {
                log(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ').success + '[ ' + String(i + 1).padEnd(String(this.#testCasesRun.length).length, ' ') + ' ] ' + testCase, argv.reportLevel);
                this.#testSuiteResults.PASSED.push(testCase);
                if (argv.reportLevel == 'INFO') {
                    createReport = true;
                }
            } else if (testCaseResult == 'WARNING') {
                log(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ' + '[ ' + String(i + 1).padEnd(String(this.#testCasesRun.length).length, ' ') + ' ] [ ' + testCaseResultObject.testCaseReultDescription + ' ] ').warn + testCase, argv.reportLevel);
                this.#testSuiteResults.WARNING.push(testCase);
                if (argv.reportLevel == 'INFO' || argv.reportLevel == 'WARN') {
                    createReport = true;
                }
            } else {
                let errParts = testCaseResultObject.testCaseReultDescription.split('http://127.0.0.1:' + + String(this.#workspace.getWorkspacePort())).join(path.resolve(this.#workspacePath)).split('\n');
                log(('[ ' + testCaseResult.padEnd(padLength, ' ') + ' ] ' + '[ ' + String(i + 1).padEnd(String(this.#testCasesRun.length).length, ' ') + ' ] [ ' + errParts[0] + ' ] ').error + testCase, argv.reportLevel);
                if (errParts.length > 1) {
                    errParts.slice(1).forEach(item => {
                        log(''.padEnd(padLength + 7, ' ') + item, argv.reportLevel);
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
                this.#createImages(testCaseResultPath, testCase, testCaseData, undefined);
                if (testCaseResult == 'FAILED') {
                    let sha;
                    if (testCaseResult == 'FAILED') {
                        try {
                            if (this.#refurl === undefined) {
                                let vizzuMinJs = 'vizzu.min.js';
                                let vizzuJs = 'vizzu.js';
                                let shaUrl = await fetch(remoteStableBucket + '/lib/sha.txt');
                                sha = await shaUrl.text();
                                sha = sha.trim();
                                if (await this.#isUrlExist(remoteLatestBucket + '/lib-' + sha + '/' + vizzuMinJs)) {
                                    this.#refurl = remoteLatestBucket + '/lib-' + sha + '/' + vizzuMinJs;
                                } else if (await this.#isUrlExist(remoteLatestBucket + '/lib-' + sha + '/' + vizzuJs)) {
                                    this.#refurl = remoteLatestBucket + '/lib-' + sha + '/' + vizzuJs;
                                } else {
                                    throw new Error('ENOENT: ' + remoteLatestBucket + '/lib-' + sha + '/' + vizzuMinJs + '|' + remoteLatestBucket + '/lib-' + sha + '/' + vizzuJs);
                                }
                            }
                            let testCaseRefData = await this.#runTestCaseClient(testCase, this.#refurl);
                            let diff = false;
                            for (let i = 0; i < testCaseData.hashes.length; i++) {
                                for (let j = 0; j < testCaseData.hashes[i].length; j++) {
                                    if (testCaseData.hashes[i][j] != testCaseRefData.hashes[i][j]) {
                                        log(''.padEnd(padLength + 5, ' ') + '[ ' + 'step: ' + i + '. - seek: ' + testCaseData.seeks[i][j] + ' - hash: ' + testCaseData.hashes[i][j].substring(0,7) + ' ' + '(ref: ' + testCaseRefData.hashes[i][j].substring(0,7) + ')' + ' ]', argv.reportLevel);
                                        diff = true
                                    }
                                }
                            }
                            if (!diff) {
                                log(''.padEnd(padLength + 5, ' ') + '[ the currently counted hashes are the same, the difference is probably caused by the environment ]', argv.reportLevel);
                            }
                            this.#createImages(testCaseResultPath, testCase, testCaseRefData, testCaseData);
                        } catch (err) {
                            let libSha = '';
                            if(typeof sha !== 'undefined') {
                                libSha = ' with lib-' + sha;
                            }
                            log(('[ ' + 'WARNING'.padEnd(padLength, ' ') + ' ] ' + '[ ' + 'can not create ref' + libSha + ' (' + err.toString() + ') ] ').warn + testCase, argv.reportLevel);
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

        #getTestCaseRefHash(testCase) {
            try {
                if (this.#testCasesData[testCase]['refs'] !== undefined) {
                    if (typeof this.#testCasesData[testCase]['refs'] === 'object' && this.#testCasesData[testCase]['refs'] !== null && Array.isArray(this.#testCasesData[testCase]['refs'])) {
                        if (this.#testCasesData[testCase]['refs'].length !== 0) {
                            return this.#testCasesData[testCase]['refs'];
                        }
                    }
                }
                return undefined;
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
                let hashes = this.#getTestCaseRefHash(testCase);
                this.#testCasesData[testCase]['refs'] = [ testCaseData.hash.substring(0,7) ];
                if (hashes === undefined) {
                    return { testCaseResult: 'WARNING', testCaseReultDescription: 'ref hash does not exist' };
                } else {
                    if (!hashes.includes(testCaseData.hash.substring(0,7))) {
                        return { testCaseResult: 'FAILED', testCaseReultDescription: 'hash: ' + testCaseData.hash.substring(0,7) + ' ' + '(ref: ' + hashes.toString() + ')' };
                    }
                }
            }
            return { testCaseResult: 'PASSED' };
        }

        #createImages(testCaseResultPath, testCase, testCaseData, dataToBeCompared) {
            let fileAdd = ''
            if (dataToBeCompared === undefined) {
                fileAdd = '-2new'
            } else {
                fileAdd = '-1ref'
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
                        } else {
                            if (dataToBeCompared !== undefined) {
                                const img1 = PNG.sync.read(Buffer.from(dataToBeCompared.images[i][j].substring(22), "base64"));
                                const img2 = PNG.sync.read(Buffer.from(testCaseData.images[i][j].substring(22), "base64"));
                                const {width, height} = img1;
                                const diff = new PNG({width, height});
                                const difference = pixelmatch(img1.data, img2.data, diff.data, width, height, {threshold: 0.01});
                                fs.writeFileSync(testCaseResultPath + '/' + path.basename(testCaseResultPath) + '_' + i.toString().padStart(3, '0') + '_' + seek[0].padStart(3, '0') + '.' + seek[1].padEnd(3, '0') + '%' + '-3diff' + '.png', PNG.sync.write(diff));
                            }
                        }
                    });
                }
            }
        }

        #createJson(testSuiteResultPath, testCases) {
            fs.mkdirSync(testSuiteResultPath, { recursive: true });
            let testCasesData = {};
            testCases.forEach(testCase => {
                if (argv.t == 'FAILED' && !this.#testSuiteResults.FAILED.includes(testCase)) return;
                testCasesData[testCase] = this.#testCasesData[testCase];
            });
            if (Object.keys(testCasesData).length !== 0) {
                testCasesData = JSON.stringify(testCasesData, null, 4);
                fs.writeFile(testSuiteResultPath + 'test_cases.json', testCasesData, (err) => {
                    if (err) {
                        throw err;
                    }
                });
            }
        }
    }

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
        .alias('c', 'cases')
        .describe('c', 'Change reference hashes json file path')
        .nargs('c', 1)
        .default('c', '/test/integration/test_cases.json')
        .boolean('b')
        .alias('b', 'disableHeadlessBrowser')
        .default('b', false)
        .describe('b', 'Disable to use headless browser')
        .alias('r', 'reportLevel')
        .choices('r', ['INFO', 'WARN', 'ERROR', 'DISABLED'])
        .default('r', 'INFO')
        .describe('r', 'Set report detail level')
        .choices('t', ['ALL', 'FAILED', 'DISABLED'])
        .default('t', 'FAILED')
        .describe('t', 'Set test_report/results/test_cases.json detail level')
        .alias('u', 'vizzuUrl')
        .describe('u', 'Change vizzu.js url')
        .nargs('u', 1)
        .default('u', '/example/lib/vizzu.js')
        .argv;

    let test = new TestSuite(__dirname + '/test_cases');
    test.runTestSuite();
} catch (err) {
    process.exitCode = 1;
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    log('[ ' + 'ERROR'.padEnd(padLength, ' ') + ' ] ' + errMsg, argv.reportLevel);
}
