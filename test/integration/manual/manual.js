const remoteLatestBucket = 'vizzu-lib-main-sha.storage.googleapis.com';
const remoteStableBucket = 'vizzu-lib-main.storage.googleapis.com';
const remoteCloudFunctions = 'europe-west3-vizzu-ci.cloudfunctions.net';


const fs = require('fs');
const path = require('path');
const yargs = require('yargs');
const express = require('express');
const fetch = require('node-fetch');


class Manual {

    #server;
    #port;

    #rootPath = path.join(__dirname, '/../../../');
    #testPath = path.join(this.#rootPath, 'test/integration/test_cases');
    #testList = [];


    constructor(argv) {
        this.#server = express();
        this.#port = String(argv.port)

        this.#server.use(express.static(this.#rootPath));

        this.#getLibList();
        this.#getTestList();

        this.#server.listen(this.#port);
        console.log('[ HOSTING ]' + ' ' + '[ ' + 'http://127.0.0.1:' + this.#port + '/test/integration/manual' + ' ]' + ' press CTRL + C to stop');
    }

    #getLibList() {
        this.#server.get('/getLibList', (req, res) => {
            let libList = { localhost: 'http://127.0.0.1:' + this.#port + '/example/lib' };
            libList['HEAD'] = 'https://' + remoteStableBucket + '/lib';
            fetch('https://' + remoteCloudFunctions + '/getVizzuList').then(vizzuListUrl => {
                vizzuListUrl.json().then(vizzuList => {
                    vizzuList.slice().reverse().forEach(vizzu => {
                        libList[vizzu.time.substring(0,10) + ' ' + vizzu.time.substring(11,16)  + ' ' + vizzu.sha] = 'https://' + remoteLatestBucket + '/' + vizzu.sha;
                    });
                    //libList['0.3.1'] = 'https://cdn.jsdelivr.net/npm/vizzu@0.3.1/dist';
                    //libList['0.3.0'] = 'https://cdn.jsdelivr.net/npm/vizzu@0.3.0/dist';
                    libList['0.2.0'] = 'https://vizzuhq.github.io/vizzu-beta-release/0.2.0';
                    res.send(libList);
                }).catch((err) => { 
                    console.log(err)
                });
            }); 
        });
    }

    #getTestList() {
        this.#server.get('/getTestList', (req, res) => {
            this.#testList = [];
            this.#setTestList(this.#testPath);
            res.send(this.#testList);
        });
    }

    #setTestList(testPath) {
        if (fs.lstatSync(testPath).isDirectory()) {
            let files = fs.readdirSync(testPath);
            files.forEach(file => {
                if (fs.lstatSync(testPath + '/' + file).isDirectory()) {
                    this.#setTestList(testPath + '/' + file);
                }
                else {
                    if (path.extname(file) == '.mjs') {
                        this.#testList.push(path.relative(this.#testPath, testPath + '/' + path.parse(file).name));
                    }
                }
            })
        }
    }
}


try {
    var argv = yargs
        .usage('Usage: [options]')
        .help('h')
        .alias('h', 'help')
        .version('0.0.1')
        .alias('v', 'version')
        .alias('p', 'port')
        .describe('p', 'Change port')
        .nargs('p', 1)
        .default('p', '8080')
        .argv;

    let manual = new Manual(argv);
} catch (err) {
    console.error(err);
    process.exitCode = 1;
}  
