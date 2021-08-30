import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
                    y: { attach: ['$exists', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    noop: { attach: ['Year'] },
                    label: { attach: ['Year'] }
                },
                title: 'Scatter Plot',
                legend: null,
                geometry: 'circle',
                coordSystem: 'cartesian'
            }
        }
    )
];

export default testSteps;