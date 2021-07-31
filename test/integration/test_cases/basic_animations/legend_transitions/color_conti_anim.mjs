import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { circleMinRadius: 5, circleMaxRadius: 0.05 },
    plot: {
        marker: {
            guides: { color: '#e8e8e8FF', lineWidth: 0.5, },
            label: { fontSize: 10, position: 'top', filter: 'lightness(0.1)', numberFormat: 'grouped' }
        },
        axis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } }
    },
};

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
            },
            style: styles
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