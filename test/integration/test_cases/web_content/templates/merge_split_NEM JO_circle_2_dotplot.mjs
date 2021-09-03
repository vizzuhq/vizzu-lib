import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels:
                {
                    x: { set: ['Joy factors', 'Value 2 (+)'], range: '0,1.1,%' },
                    y: { set: ['Value 5 (+/-)'], range: '-0.1,1.1,%' },
                    color: { set: ['Joy factors'] },
                    size: { set: ['Value 2 (+)'] },
                    label: { set: ['Country_code'] }
                },
                title: 'Correlation',
                geometry: 'circle'
            },
            style: {data:{circleMaxRadius: 0.02}}
        }
    ),
    chart => chart.animate(
        {
            config: {
                title: 'Stacked Comparison',
                split: true,
                orientation: 'vertical',
                align: 'min'
            }
        }
    ),
    chart => chart.animate(
        {
            config: {
                channels:
                {
                    size: { set: ['Value 2 (+)'] }
                },
                title: 'Comparison',
                split: false,
            }
        }
    )
];

export default testSteps;