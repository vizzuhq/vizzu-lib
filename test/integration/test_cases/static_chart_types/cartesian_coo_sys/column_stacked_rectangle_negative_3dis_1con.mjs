import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)', 'Country_code'] },
                    x: { attach: ['Year', 'Joy factors'] },
                    color: { attach: ['Country_code'] },
                    size: { attach: ['Country_code', 'Value 2 (+)'] },
                    noop: { attach: ['Year'] }
                },
                title: 'Stacked Column Chart with (-) Nums'
            },
            style: {
                data: 
                { 
                    columnMaxPadding: 0.1 
                },
                plot: 
                {
                    marker: 
                    {
                        borderWidth: 1,
                        borderOpacity: 1
                    }
                }
            }
        }
    )
];

export default testSteps;