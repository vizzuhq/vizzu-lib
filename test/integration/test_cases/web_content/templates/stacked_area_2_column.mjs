import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'], range: '0,1.1,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] },
                },
                title: 'Time distribution',
                geometry: 'area'
            }
        }
    ),
    chart => chart.animate(
        {
            config: 
            {
                split: true
            }
        }
    ),
        chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { detach: ['Joy factors'], attach: ['Year']  },
                    x: { detach: ['Year'], attach: ['Joy factors'] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                split: false
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
            geometry: { delay: 0.43, duration: 1 },
            y: { delay: 1, duration: 2 },
            x: { delay: 0, duration: 1 }
        }
    )
];

export default testSteps;