import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Year'] },
                    x: { attach: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '135%' } },
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
                    
                    },
                    marker: 
                    {
                        borderOpacity: 1 
                    } 
                }
            }
        }
    )
];

export default testSteps;