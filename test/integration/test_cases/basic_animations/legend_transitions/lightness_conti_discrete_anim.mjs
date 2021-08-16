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
            },
            style: styles
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    lightness: { attach: ['Neg_small'] }
                }
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
                }
            }
        }
    )
];

export default testSteps;