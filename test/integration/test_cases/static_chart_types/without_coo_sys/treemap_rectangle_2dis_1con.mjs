import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: ['Joy factors'] },
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Treemap (Stacked)'
            }
        }
    )
];

export default testSteps;