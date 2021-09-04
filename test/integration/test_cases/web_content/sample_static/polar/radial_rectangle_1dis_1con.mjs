import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors'], range: '-0.3,1,%' },
                    x: { attach: ['Value 2 (+)'], range: '0,1.33,%' },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Radial Bar Chart',
                coordSystem: 'polar'
            },
            style:
            {
                plot:
                { 
                  yAxis:
                    { 
                        label:
                        { 
                            paddingRight: 20
                        }
                    
                    },                    
                  xAxis:
                    {
                        title:
                        {
                            color: '#ffffff00'
                        },
                        label:
                        {
                            color: '#ffffff00'
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;