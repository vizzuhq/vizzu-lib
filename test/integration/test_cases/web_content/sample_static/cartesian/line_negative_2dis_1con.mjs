import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 5 (+/-)'] },
                color: { set: ['Country'] }
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    })
];

export default testSteps;