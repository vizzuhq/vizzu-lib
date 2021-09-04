import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['Value 2 (+)'], range: { min: '0%', max: '110%' } },
                    label: { attach: ['Value 2 (+)'] },
                },
                title: 'Area Chart',
                align: 'none',
                geometry: 'area'
            }
        }
    )
];

export default testSteps;