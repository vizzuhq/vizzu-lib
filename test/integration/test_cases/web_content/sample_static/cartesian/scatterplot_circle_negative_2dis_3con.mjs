import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: { min: '-10%', max: '110%' } },
                    x: { attach: ['Value 6 (+/-)'], range: { min: '-10%', max: '110%' } },
                    color: { attach: ['Country'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Bubble Plot',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;