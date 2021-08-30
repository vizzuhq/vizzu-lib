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
                    y: { attach: ['$exists', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Scatterplot',
                align: 'none',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;