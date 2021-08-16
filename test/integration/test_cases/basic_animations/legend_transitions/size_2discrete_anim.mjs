import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { circleMinRadius: 5, circleMaxRadius: 0.05 },
    plot: {
        marker: {
            guides: { color: '#e8e8e8FF', lineWidth: 0.5, },
            label: { fontSize: 10, position: 'top', filter: 'lightness(0.1)', numberFormat: 'grouped' }
        },
        xAxis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } },
        yAxis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } }
    }
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
                    size: { attach: ['Country_code', 'Neg_small'] },
                    label: { attach: ['Year'] }
                },
                title: 'Size 1 Discrete series.',
                legend: 'size',
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
                    size: { attach: ['Year'] }
                },
                title: 'Size 2 Discrete series (stacked).'
            }
        }
    )
];

export default testSteps;