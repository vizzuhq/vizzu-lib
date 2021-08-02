import {initSlider} from './control.js';

let queryString = window.location.search;
let urlParams = new URLSearchParams(queryString);
let testCase = urlParams.get('testCase');
let vizzuUrl = urlParams.get('vizzuUrl');
let chart;

import(vizzuUrl + '/vizzu.js').then(vizzuModule => {
    let Vizzu = vizzuModule.default;
    chart = new Vizzu('vizzuCanvas');
    return chart.initializing;
}).then(chart => {
    console.log(chart.version());
    initSlider(chart);
    return import('/test/integration/test_cases/' + testCase + '.mjs')
}).then(testModule => {
    let anim = chart.initializing;
    for (let step of testModule.default) anim = anim.then(step);
}).catch(console.log);
