import {initSlider} from './control.js';

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testFile = urlParams.get('testFile');
let testType = urlParams.get('testType');
let testIndex = urlParams.get('testIndex');
let vizzuUrl = urlParams.get('vizzuUrl');
let canvas = document.getElementById('vizzuCanvas');
let chart;

if (!vizzuUrl.endsWith('/vizzu.js') && !vizzuUrl.endsWith('/vizzu.min.js')) {
    vizzuUrl = vizzuUrl + '/vizzu.js';
}

import(vizzuUrl).then(vizzuModule => {
    let Vizzu = vizzuModule.default;
    chart = new Vizzu(canvas);
    return chart.initializing;
}).then(chart => {
    chart.module._vizzu_setLogging(true);
    console.log(chart.version());
    initSlider(chart);
    return import(testFile + '.mjs');
}).then(testModule => {
    let anim = chart.initializing;
    let testSteps = [];
    if (testType === 'single') {
        testSteps = testModule.default;
    } else if (testType === 'multi') {
        testSteps = testModule.default[testIndex].testSteps;
    }
    for (let step of testSteps) anim = anim.then(step);
    return anim;
}).then(anim => {
    let ctx = canvas.getContext('2d');
    document.vizzuImgData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    return anim;
}).catch(console.log);
