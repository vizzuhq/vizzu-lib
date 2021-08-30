import { data } from '/test/integration/test_cases/web_content/content_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'], range: '0,1.075,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Coxcomb Chart',
                coordSystem: 'polar'
            },
            style: 
            {
                data: 
                { 
                    columnMaxPadding: 0.21 
                },
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