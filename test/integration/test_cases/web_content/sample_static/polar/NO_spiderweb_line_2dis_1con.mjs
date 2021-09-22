import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)'] },
                color: { set: ['Country'] }
            },
            title: 'Polar Chart',
            geometry: 'line',
            coordSystem: 'polar'
        }
    })
];

export default testSteps;