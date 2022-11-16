import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Value 2 (+)', 'Joy factors'] },
                    y: { attach: 'Year' },
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Value 2 (+)' }
                },
                title: 'Splitted Bar Chart',
                split: true
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;