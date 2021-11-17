import { data } from '/test/integration/test_data/source _code_data.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: 'D1',
                size: ['Line count', 'Unique file name'],
                lightness: 'Line count'
            },
            title: 'Vizzu Source Code by Line Count'
        },
        style: { 'fontSize': '80%' }
    }),
    chart => chart.animate({
        lightness: 'Line count'
    }),
    chart => chart.animate({
        config: {
            y: ['D1','Unique file name'],
            x: 'Line count',
//            sort: 'byValue',
            reverse: true
        }        ,
        style: { 'plot.paddingLeft': '100' }
    }),
    chart => chart.animate({
        data: { filter: record => record.D1 == 'test', },
        config: {
            channels: {
                y: 'Unique file name',
            },
            legend: null
        }
    })];
export default testSteps;