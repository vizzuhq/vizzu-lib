import { data } from '/test/integration/test_data/chart_types_eu.js';

let styles = {
    data: { circleMinRadius: 5, circleMaxRadius: 0.05 },
    plot: {
        marker: {
            guides: { color: '#e8e8e8FF', lineWidth: 0.5, },
            label: { fontSize: 10, position: 'top', filter: 'lightness(-0.15)', numberFormat: 'grouped' }
        },
        xAxis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } },
        yAxis: { ticks: { color: '#e8e8e8FF', lineWidth: 1 } }
    },
    data: {
        colorGradient: '#4171cd 0, #8536c7 0.25, #c33e7f 0.5, #e9974e 0.75, #f3f239 1'
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
                    y: { attach: ['Negative nums'], range: '0,1.5,%' },
                    x: { attach: ['Neg_huge'], range: '0,1.5,%' },
                    lightness: { attach: ['Country'] },
                    color: { attach: ['Negative nums'] },
                    size: { attach: ['Neg_small'] },
                    label: { attach: ['Negative nums'] }
                },
                title: 'Default Color Gradient',
                legend: 'color',
                geometry: 'circle'
            },
            style: styles
        }
    )
];

export default testSteps;