import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { set: ['Joy factors'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: ['Country_code'] }
                },
                title: 'Comparison',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: ['Joy factors', 'Value 6 (+/-)'], range: '0,1.05,%' /*, range: '-20000000,10000000,1'*/ },
                    y: { set: ['Value 5 (+/-)'], range: '0,1.05,%' }
                },
                title: 'Correlation'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    x: { set: null },
                    y: { set: null },
                    size: { set: ['Value 2 (+)', 'Country_code'] }
                },
                title: 'Stacked Comparison'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    size: { set: ['Value 2 (+)'] }
                },
                title: 'Comparison'
            }
        }
    )
];

export default testSteps;