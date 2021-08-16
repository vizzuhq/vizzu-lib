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
                    color: { attach: ['Neg_small'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Country'] }
                },
                title: 'Color Continuous series Changing Anim.',
                legend: 'color',
                geometry: 'circle'
            },
            style:
            {
                data: { circleMinRadius: 5, circleMaxRadius: 0.05 },
                plot: {
                    marker: {
                        guides: { color: '#e8e8e8FF', lineWidth: 0.5, },
                        label: { fontSize: 10, position: 'top', filter: 'lightness(0.1)', numberFormat: 'grouped' }
                    },
                    axis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } }
                },
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    color: { detach: ['Neg_small'], attach: ['Negative nums'] }
                }
            }
        }
    )
];

export default testSteps;