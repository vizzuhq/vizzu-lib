import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Negative nums'], range: '0,1.1,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Size Continuous Anim.',
                legend: 'size',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                legend: null
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                legend: 'size'
            }
        }
    )
];

export default testSteps;