import { VideoRecorder } from "./videorecorder.js";


function catchError(err) {
    console.error(err)
    let errMsg = err.toString();
    if (err.stack !== undefined) {
        errMsg = err.stack;
    }
    window.result = { result: "ERROR", description: errMsg };
    document.title = "Finished";
}


try {
    let queryString = window.location.search;
    let urlParams = new URLSearchParams(queryString);
    let testSuitePath = urlParams.get("testSuitePath");
    let testCasesPath = urlParams.get("testCasesPath");
    let testCase = urlParams.get("testCase");
    let testName = urlParams.get("testName");
    let testType = urlParams.get("testType");
    let testIndex = urlParams.get("testIndex");
    let vizzuUrl = urlParams.get("vizzuUrl");

    import(vizzuUrl).then(vizzuModule => {
        var Vizzu = vizzuModule.default;
        return import(testSuitePath + "/" + testCasesPath + "/" + testCase + ".mjs").then((testCasesModule) => {
            let chart = new Vizzu("vizzuCanvas");
            let videoRecorder = new VideoRecorder("vizzuCanvas", (data) => {
                let a = document.createElement("a")
                a.setAttribute("href", data)
                a.setAttribute("download", testCasesPath.replace("/", "__") + "__" + testName + ".webm")
                a.click()
                window.result = { result: "OK" };
                document.title = "Finished";
            });
            let testSteps = [];
            if (testType === "single") {
                testSteps = testCasesModule.default;
            } else if (testType === "multi") {
                testSteps = testCasesModule.default[testIndex]["testSteps"];
            }
            let steps = [];
            let snapshots = [];
            let snapshotCnt = 0;
            steps.push(chart => {
                chart.setAnimation(null);
                return testSteps[0](chart);
            });
            steps.push(chart => { 
                chart.render.updateFrame(true);
                videoRecorder.start();
                return chart;
            });
            for (let i = 1; i < testSteps.length; i++)
            {
                snapshotCnt++;
                steps.push(chart => {
                    snapshots.push(chart.store());
                    return testSteps[i](chart);
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