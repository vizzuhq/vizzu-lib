import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Value 6 (+/-)'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Value 4 (+/-)'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Size Continuous Anim.',
                legend: 'size',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                legend: null
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                legend: 'size'
            }
        }
    )
];

export default testSteps;