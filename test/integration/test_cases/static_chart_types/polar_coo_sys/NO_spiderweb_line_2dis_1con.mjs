import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Positive nums'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Country'] }
                },
                title: 'Spiderweb (fake. no multiaxes)',
                geometry: 'line',
                coordSystem: 'polar'
            }
        }
    )
];

export default testSteps;