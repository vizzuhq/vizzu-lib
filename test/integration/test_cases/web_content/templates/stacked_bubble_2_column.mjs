import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Country_code', 'Value 2 (+)'] }
                },
                title: 'Fragmented',
                geometry: 'circle'
            }
        }
    ),
        chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    x: { attach: ['Joy factors'] },
                    y: { attach: ['Country_code', 'Value 2 (+)' ], range: '0,1.1,%' },

                    label: { detach: ['Country_code'] },
                    size: { detach: ['Country_code', 'Value 2 (+)'] }
                },
                title: 'Amount',
                geometry: 'rectangle',
                orientation: 'vertical'
            },
            style: {
                plot: {
                    marker: {
                        borderOpacity: 1
                    }
                }
            }
        }
    )
];

export default testSteps;