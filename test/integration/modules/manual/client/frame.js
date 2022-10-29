let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testFile = urlParams.get('testFile');
let testType = urlParams.get('testType');
let testIndex = urlParams.get('testIndex');
let vizzuUrl = urlParams.get('vizzuUrl');
let slider = document.getElementById('myRange');
let canvas = document.getElementById('vizzuCanvas');
let chart;
let testSteps = [];
let snapshotId = undefined;

function snapshot(value)
{
    if (snapshotId != value) return;
    console.log('snapshot')
    let ctx = canvas.getContext('2d');
    document.vizzuImgIndex = 2*value;
    document.vizzuImgData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    document.vizzuImgIndex = 2*value+1;
}

function setSlider(value)
{
	let t = value;
	chart.animation.pause();
	chart.animation.seek(t / 10 + '%');
    snapshotId = value;
    setTimeout(() => { snapshot(value) }, 15);
}

function initSlider() {
	slider.addEventListener('input', (e) => { 
        setSlider(e.target.value); 
    });
	chart.on('update', (ev) => { slider.value = ev.data.progress * 1000; })
};

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
    initSlider();
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
}).catch(console.log);

function run(chartToRun)
{
    if (!chartToRun) chartToRun = chart;
    chartToRun.animation.play();
}
