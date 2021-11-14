import { data } from '/test/integration/test_data/source _code_data.js';
const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: 'D1',
                size: ['Line count', 'File name'],
                lightness: 'Line count'
            },
            title: 'Vizzu Source Code by Line Count'
        },
        style: { 'fontSize': '80%' }
    }),
    chart => chart.animate({
        data: { filter: record => record.D1 == 'test', },
        config: { channels: { label: 'File name', } }
    }),
    chart => chart.animate({
            config: {
                y: ['Unique', 'File name'],
                x: 'Line count',
                label: 'Line count'
            },
            sort: 'byValue',
            reverse: true // ?miért nem reverse?
    })];
export default testSteps;