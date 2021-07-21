//import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';
//import Vizzu from '/example/lib/vizzu.js'


function catchError(err) {
    console.log(err)
    window.results = { result: 'ERROR' };
}

function digestMessage(message) {
    let msgUint8 = new TextEncoder().encode(message);
    return crypto.subtle.digest('SHA-256', msgUint8).then(hashBuffer => {
        let hashArray = Array.from(new Uint8Array(hashBuffer));
        let hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
        return hashHex;
    });
}

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testCase = urlParams.get('testCase');
let vizzuUrl = urlParams.get('vizzuUrl');
let results = { result: 'PASSED', seeks: [], images: [], hashes: [] };

import(vizzuUrl + '/vizzu.js').then((vizzuModule) => {
    var Vizzu = vizzuModule.default;
    return import('/test/integration/testCases/' + testCase + '.mjs').then((testCasesModule) => {
        return fetch('/test/integration/testCases/' + testCase + '.json')
            .then(rawhashList => rawhashList.json())
            .then(hashList => {
                let chart = new Vizzu('vizzuCanvas');
                return chart.initializing.then((chart) => {
                    let promise = Promise.resolve(chart);
                    let promises = [];
                    for (let i = 0; i < testCasesModule.default.length; i++) {
                        promise = promise.then((chart) => {
                            let prom = testCasesModule.default[i](chart)
                            let anim = chart.animation;
                            anim.pause();
                            let seeks = Object.keys(hashList[i])
                            seeks.sort(function(a, b) {
                                return parseInt(a.replace('%', '')) - parseInt(b.replace('%', ''));
                            });
                            results.seeks[i] = [];
                            results.images[i] = [];
                            results.hashes[i] = [];
                            seeks.forEach(key => {
                                let seek = key.replace('%', '') + '%'
                                results.seeks[i].push(seek);
                                let hash = hashList[i][key]
                                anim.seek(seek);
                                chart.render.updateFrame(true);
                                let cavasElement = document.getElementById('vizzuCanvas');
                                let dataURL = cavasElement.toDataURL();
                                results.images[i].push(dataURL);
                                let digest = digestMessage(dataURL);
                                digest = digest.then(digestBuffer => {
                                    results.hashes[i].push(digestBuffer);
                                    if (hash == digestBuffer) {
                                        console.log(testCase + ' : ' + i + ' : ' + seek + ' : ' + 'PASSED');
                                    } else {
                                        results.result = 'FAILED';
                                        console.error(testCase + ' : ' + i + ' : ' + seek + ' : ' + 'FAILED' + ' : ' + digestBuffer);
                                    }
                                });
                                promises.push(digest);
                            });
                            anim.play();
                            return prom 
                        });
                    }
                    return promise.then(() => {
                        return Promise.all(promises).then(() => {
                            if (typeof window.results === 'undefined') {
                                window.results = results;
                            }
                        });
                    });
                });
            });
    });
}).catch((err) => { catchError(err) });
