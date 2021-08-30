import { data } from '/test/integration/test_cases/web_content/content_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
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