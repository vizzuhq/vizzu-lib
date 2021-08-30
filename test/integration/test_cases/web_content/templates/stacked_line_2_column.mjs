import { data } from '/test/integration/test_cases/web_content/content_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Value 2 (+)'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Time distribution',
                geometry: 'line'
            }
        }
    ),
        chart => chart.animate(
        {
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Year']  },
                    x: { detach: ['Year'], attach: ['Joy factors'] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                sort: 'experimental'
            },
            style: {
                plot: {
                    marker: {
                        borderOpacity: 1
                    }
                }
            }
        },
        {
            geometry: { delay: 0.4, duration: 0.8 },
            y: { delay: 1, duration: 2 },
            x: { delay: 0, duration: 1 }
        }
    )
];

export default testSteps;