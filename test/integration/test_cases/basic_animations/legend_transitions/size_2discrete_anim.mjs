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
                    size: { attach: ['Country_code', 'Neg_small'] },
                    label: { attach: ['Year'] }
                },
                title: 'Size 1 Discrete series.',
                legend: 'size',
                geometry: 'circle'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    label: { detach: ['Year'] },
                    shape: { attach: ['Year'] }
                },
                title: 'Size 1 Discrete series..'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    size: { attach: ['Year'] },
                    label: { attach: ['Year'] }
                },
                title: 'Size 2 Discrete series (stacked).'
            }
        }
    )
];

export default testSteps;