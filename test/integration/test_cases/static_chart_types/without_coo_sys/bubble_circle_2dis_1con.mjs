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
                    size: { attach: ['Country_code', 'Value 2 (+)'] },
                    label: { attach: ['Country_code'] }
                },
                title: 'Bubble chart',
                geometry: 'circle'
            },
            style: 
            {
                plot: 
                {
                    marker: 
                    {
                        label: 
                        {
                            fontSize: 9
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;