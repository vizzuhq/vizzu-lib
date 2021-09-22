import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Country'] },
                color: { set: ['Country'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Bar Chart',
            orientation: 'vertical'
        }
    })
];

export default testSteps;