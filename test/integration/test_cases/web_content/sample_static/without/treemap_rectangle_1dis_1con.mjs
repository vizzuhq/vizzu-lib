import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                label: { set: ['Country_code'] },
                size: { set: ['Value 2 (+)'] },
            },
            title: 'Treemap'
        }
    })
];

export default testSteps;