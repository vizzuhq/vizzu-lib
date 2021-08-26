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
                    color: { attach: ['Negative nums'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Country'] }
                },
                title: 'Color Continuous Anim.',
                legend: 'color',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            data: {
                filter: record => record.Country != 'Germany' && record.Country != 'Italy' && record.Country != 'Hungary' && record.Country != 'Netherlands',
            },
            descriptor: {
                title: 'Color Continuous Anim. - Filtered'
            }
        }
    )
];

export default testSteps;