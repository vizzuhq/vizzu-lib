function catchError(err) {
    console.error(err)
    window.testData = { result: 'ERROR', description: err.toString() };
}

function digestMessage(message) {
    return crypto.subtle.digest('SHA-256', message).then(hashBuffer => {
        let hashArray = Array.from(new Uint8Array(hashBuffer));
        let hashHex = hashArray.map(b => b.toString(16).padStart(2, '0')).join('');
        return hashHex;
    });  
}

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testCase = urlParams.get('testCase');
let vizzuUrl = urlParams.get('vizzuUrl');
let animstep = urlParams.get('animstep');
let testData = { result: 'FINISHED', seeks: [], images: [], hashes: [], hash: '' };

import(vizzuUrl + '/vizzu.js').then((vizzuModule) => {
    var Vizzu = vizzuModule.default;
    return import('/test/integration/test_cases/' + testCase + '.mjs').then((testCasesModule) => {
        let seeks = [];
        for (let seek = parseFloat(animstep); seek <= 100; seek += parseFloat(animstep)) {
            seeks.push(seek);
        }
        let chart = new Vizzu('vizzuCanvas');
        return chart.initializing.then((chart) => {
            let promise = Promise.resolve(chart);
            let promises = [];
            for (let i = 0; i < testCasesModule.default.length; i++) {
                promise = promise.then((chart) => {
                    let prom = testCasesModule.default[i](chart)
                    let anim = chart.animation;
                    anim.pause();
                    testData.seeks[i] = [];
                    testData.images[i] = [];
                    testData.hashes[i] = [];
                    seeks.forEach(seek => {
                        seek = seek + '%'
                        testData.seeks[i].push(seek);
                        anim.seek(seek);
                        chart.render.updateFrame(true);
                        let canvasElement = document.getElementById('vizzuCanvas');
                        let dataURL = canvasElement.toDataURL();
                        testData.images[i].push(dataURL);
                        let ctx = canvasElement.getContext('2d');
                        let digestData = ctx.getImageData(0, 0, canvasElement.width, canvasElement.height);
                        let digest = digestMessage(digestData.data.buffer.slice());
                        digest = digest.then(digestBuffer => {
                            testData.hashes[i].push(digestBuffer);
                        });
                        promises.push(digest);
                    });
                    anim.play();
                    return prom 
                });
            }
            return promise.then(() => {
                return Promise.all(promises).then(() => {
                    testData.hashes.forEach(items => {
                        testData.hash += items.join('');
                    });
                    var buf = new ArrayBuffer(testData.hash.length*2);
                    var bufView = new Uint16Array(buf);
                    for (var i=0, strLen=testData.hash.length; i < strLen; i++) {
                        bufView[i] = testData.hash.charCodeAt(i);
                    }
                    digestMessage(bufView).then(hash => {
                        testData.hash = hash;
                        if (typeof window.testData === 'undefined') {
                            window.testData = testData;
                        }
                    });
                });
            });
        });
    });
}).catch((err) => { catchError(err) });
