import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Year'], range: '-0.2,1,%' },
                    x: { attach: ['Country', 'Value 2 (+)'], range: '0,1.35,%' },
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