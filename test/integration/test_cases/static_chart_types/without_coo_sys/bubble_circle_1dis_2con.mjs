import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    color: { attach: ['Country'] },
                    label: { attach: ['Country_code'] },
                    lightness: { attach: ['Value 5 (+/-)'] },
                    size: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Bubble Chart',
                geometry: 'circle'
            }
        }
    )
];

export default testSteps;