import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels:
            {
                y: {
                    attach: ['Country', 'Value 2 (+)'],
                    range: {
                        min: '0%', max: '110%'
                    }
                },
                x: { attach: ['Year'] },
                color: { attach: ['Country'] },
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        }
    })
];

export default testSteps;