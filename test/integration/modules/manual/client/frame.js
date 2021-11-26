import {initSlider} from './control.js';

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testCase = urlParams.get('testCase');
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
    return import(testCase + '.mjs');
}).then(testModule => {
    let anim = chart.initializing;
    for (let step of testModule.default) anim = anim.then(step);
    return anim;
}).then(anim => {
    let ctx = canvas.getContext('2d');
    document.vizzuImgData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    return anim;
}).catch(console.log);
