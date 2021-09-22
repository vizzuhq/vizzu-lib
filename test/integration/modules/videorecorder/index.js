import { VideoRecorder } from "./videorecorder.js";


function catchError(err) {
    console.error(err)
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    window.data = { result: 'ERROR', description: errMsg };
}


try {
    let queryString = window.location.search;
    let urlParams = new URLSearchParams(queryString);
    let testCase = urlParams.get('testCase');
    let vizzuUrl = urlParams.get('vizzuUrl');

    import(vizzuUrl).then(vizzuModule => {
        var Vizzu = vizzuModule.default;
        return import('/test/integration/test_cases/' + testCase + '.mjs').then((testCasesModule) => {
            let chart = new Vizzu('vizzuCanvas');
            let videoRecorder = new VideoRecorder('vizzuCanvas', (data) => {
                let a = document.createElement('a')
                a.setAttribute('href', data)
                a.setAttribute('download', testCase + '.webm')
                a.click()
                window.data = { result: 'DONE' };
            });
            let snapshot = {};
            let f = testCasesModule.default[0];
            testCasesModule.default[0] = chart => {
                chart.setAnimation(null);
                return f(chart);
            }
            testCasesModule.default.splice(1, 0, chart => { 
                snapshot.value = chart.store();
                chart.render.updateFrame(true);
                videoRecorder.start();
                return chart;
            });
            testCasesModule.default.push(chart => chart.animate(snapshot.value));

            let promise = chart.initializing;
            for (let i = 0; i < testCasesModule.default.length; i++) {
                promise = promise.then((chart) => {
                    return testCasesModule.default[i](chart);
                });
            }
            return promise.then((chart) => {
                setInterval(() => { chart.render.updateFrame(true);}, 50);
                setTimeout(() => { videoRecorder.stop(); }, 2000);
            });
        });
    }).catch((err) => { catchError(err); });
} catch (err) {
    catchError(err);
}