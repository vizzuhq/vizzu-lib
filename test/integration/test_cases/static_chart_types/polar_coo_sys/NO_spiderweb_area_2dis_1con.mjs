import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Country', 'Positive nums'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] }
                },
                title: 'Area Spiderweb (fake. no multiaxes)',
                geometry: 'area',
                coordSystem: 'polar',
                legend: 'color'
            }
        }
    )
];

export default testSteps;