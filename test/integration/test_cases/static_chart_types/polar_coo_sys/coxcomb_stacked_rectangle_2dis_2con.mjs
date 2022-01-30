import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'] },
                    x: { attach: ['Year', 'Value 2 (+)'] },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar'
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        borderOpacity:1
                    } 
                }
            }
        }
    )
];

export default testSteps;