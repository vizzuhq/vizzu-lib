import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                color: { set: ['Joy factors'] },
                size: { set: ['Value 2 (+)', 'Country_code'] },
                label: { set: ['Country_code'] },
                lightness: { set: ['Value 2 (+)'] }
            },
            title: 'Stacked Treemap'
        }
    })
];

export default testSteps;