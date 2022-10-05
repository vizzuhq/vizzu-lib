let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testFile = urlParams.get('testFile');
let testType = urlParams.get('testType');
let testIndex = urlParams.get('testIndex');
let vizzuUrl = urlParams.get('vizzuUrl');
let canvas = document.getElementById('vizzuCanvas');
let chart;
let testSteps = [];

if (!vizzuUrl.endsWith('/vizzu.js') && !vizzuUrl.endsWith('/vizzu.min.js')) {
    vizzuUrl = vizzuUrl + '/vizzu.js';
}

var setup = import(vizzuUrl).then(vizzuModule => {
    let Vizzu = vizzuModule.default;
    chart = new Vizzu(canvas);
    return chart.initializing;
}).then(chart => {
    chart.module._vizzu_setLogging(true);
    console.log(chart.version());
    initSlider(chart);
    return import(testFile + '.mjs');
}).then(testModule => {
    if (testType === 'single') {
        testSteps = testModule.default;
    } else if (testType === 'multi') {
        testSteps = testModule.default[testIndex].testSteps;
    }
    return chart.initializing;
});

setup.then(chart => {
    setTimeout(() => {
        chart.animation.pause();
    },0);
    let finished = chart.initializing;
    for (let step of testSteps) finished = finished.then(step);
    return finished;
}).then(finished => {
    let ctx = canvas.getContext('2d');
    document.vizzuImgData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    return finished;
}).catch(console.log);

function run(chart)
{
    chart.animation.play();
}
