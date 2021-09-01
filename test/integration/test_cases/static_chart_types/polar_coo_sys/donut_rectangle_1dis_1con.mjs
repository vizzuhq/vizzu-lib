import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Joy factors', 'Value 2 (+)'] },
                    y: { range: '1,-3,%' },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Donut Chart',
                orientation: 'vertical',
                coordSystem: 'polar'
            },
            style:
            {
                plot:
                { marker:
                    { label:
                        {
                            position: 'top' 
                        }
                    }
                }
            }
        }

    )
];

export default testSteps;