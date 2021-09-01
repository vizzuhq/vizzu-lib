import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
    {
        data: data,
        descriptor: {       
            channels:
            {
                color: { attach: ['Joy factors'] },
                size: { attach: ['Value 2 (+)'] },
               noop: { attach: ['Country_code'] }
            },
            title: 'Without 2 Cartesian - Bubble 2 Scatter.',
            geometry: 'circle'
        }
    }),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Value 2 (+)'] },
                    y: { attach: ['Value 5 (+/-)'] },
                    size: { detach: ['Value 2 (+)'] },
                    size: { attach: ['Value 1 (+)'] }
                }
            },
            style:{plot:{marker:{borderOpacity:0, borderWidth:0,fillOpacity:0.8}}}
        }
        ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['Value 2 (+)'] },
                    y: { detach: ['Value 5 (+/-)'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] }
                }
            }
        }
    )
];

export default testSteps;