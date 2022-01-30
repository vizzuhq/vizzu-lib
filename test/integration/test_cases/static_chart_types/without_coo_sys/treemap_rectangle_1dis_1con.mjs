import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    label: { attach: ['Country_code'] },
                    size: { attach: ['Value 2 (+)'] },
                },
                title: 'Treemap'
            }
        }
    )
];

export default testSteps;