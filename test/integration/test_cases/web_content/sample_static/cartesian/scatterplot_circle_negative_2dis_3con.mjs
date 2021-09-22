import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 6 (+/-)'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Country'] },
                size: { set: ['Value 4 (+/-)'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Bubble Plot',
            geometry: 'circle'
        }
    })
];

export default testSteps;