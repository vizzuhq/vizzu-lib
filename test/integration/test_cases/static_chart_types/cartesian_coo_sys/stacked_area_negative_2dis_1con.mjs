import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] },
                },
                title: 'Stacked Area Chart with (-) Nums',
                geometry: 'area'
            }
        }
    )
];

export default testSteps;