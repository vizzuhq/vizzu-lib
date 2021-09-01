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
                    size: { attach: ['Value 2 (+)', 'Country_code'] },
                    label: { attach: ['Country_code'] },
                    lightness: { attach: ['Value 2 (+)'] }
                },
                title: 'Treemap'
            }
        }
    )
];

export default testSteps;