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
            let steps = [];
            let snapshots = [];
            let snapshotCnt = 0;
            steps.push(chart => {
                chart.setAnimation(null);
                return testCasesModule.default[0](chart);
            });
            steps.push(chart => { 
                chart.render.updateFrame(true);
                videoRecorder.start();
                return chart;
            });
            for (let i = 1; i < testCasesModule.default.length; i++)
            {
                snapshotCnt++;
                steps.push(chart => {
                    snapshots.push(chart.store());
                    return testCasesModule.default[i](chart);
                });
            }
            for (let i = 0; i < snapshotCnt; i++)
            {
                steps.push(chart => {
                    let res = chart.animate(snapshots.at(-1));
                    snapshots.pop();
                    return res;
                });
            }

            let promise = chart.initializing;
            for (let step of steps) {
                promise = promise.then(chart => step(chart));
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