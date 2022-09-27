import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Joy factors', 'Value 2 (+)'] },
                    y: { attach: 'Value 1 (+)' },
                    color: { attach: 'Joy factors' },
//                    label: { attach: 'Value 1 (+)' }
                },
                title: 'Variable Radius Pie Chart',
                coordSystem: 'polar'
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;