import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Joy factors'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                },
                title: 'Without 2 polar - Treemap 2 Coxcomb.'
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { attach: ['Joy factors', 'index'] },
                    y: { attach: ['Value 2 (+)', 'Country_code'] },
                    size: { detach: ['Value 2 (+)'] },
                    label: { detach: ['Country_code'] }
                },
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    marker: 
                    {
                        borderOpacity: 1
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            descriptor: {
                channels:
                {
                    x: { detach: ['index', 'Joy factors'] },
                    y: { detach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Value 2 (+)'] },
                    lightness: { attach: ['Country_code'] }
                },
                coordSystem: 'cartesian'
            }
        }
    )
];

export default testSteps;