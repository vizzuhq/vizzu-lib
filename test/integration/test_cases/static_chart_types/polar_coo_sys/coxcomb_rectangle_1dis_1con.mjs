import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 2 (+)'] },
                    x: { attach: ['Joy factors'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Coxcomb Chart',
                orientation: 'horizontal',
                coordSystem: 'polar'
            },
            style:
            {
                plot:
                {
                    marker:
                    { 
                        rectangleSpacing: 0,
                        label:
                        {
                            fontSize: 12,
                            position: 'center'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;