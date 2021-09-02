import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Year'], range: '-10,20,1' },
                    x: { attach: ['Country', 'Value 2 (+)'], range: '0,1.32,%' },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Stacked Radial Bar Chart',
                coordSystem: 'polar'
            },
            style: {
                plot:
                { 
                  yAxis:
                    { 
                        label:
                        { 
                            fontSize: 8
                        }
                    
                    }
                }
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                title: 'Splitted Radial Bar Chart',
                split: true
            }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                title: 'Stacked Radial Bar Chart',
                split: false
            }
        }
    )
];

export default testSteps;