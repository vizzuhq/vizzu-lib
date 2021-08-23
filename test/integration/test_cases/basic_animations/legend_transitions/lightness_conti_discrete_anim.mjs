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
                    lightness: { attach: ['Country'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Lightness Discrete + Continuous.',
                legend: 'lightness',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    lightness: { attach: ['Neg_small'] }
                }
            }
        }
    )
];

export default testSteps;