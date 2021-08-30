import { data } from '/test/integration/test_cases/web_content/content_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    color: { attach: ['Country'] },
                    label: { attach: ['Country_code'] },
                    lightness: { attach: ['Value 5 (+/-)'] },
                    size: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Bubble Chart',
                geometry: 'circle',
                legend: null
            }
        }
    )
];

export default testSteps;