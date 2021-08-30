import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Year'] },
                    y: { attach: ['Value 2 (+)'], range: '0,1.1,%' },
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