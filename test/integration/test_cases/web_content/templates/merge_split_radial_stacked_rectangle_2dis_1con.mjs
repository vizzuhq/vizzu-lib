import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Year'],range: {min:'-3', max:'20' } },
                    x: { attach: ['Country', 'Value 2 (+)'], range: {min:'0%', max:'133%' } },
                    color: { attach: ['Country'] }
                },
                title: 'Stacked Radial Bar Chart',
                coordSystem: 'polar'
            },
            style: {
                plot:
                { 
                  yAxis:
                    { 
                        color: '#ffffff00',
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
                          },                        
                          interlacing: 
                          {
                              color: '#ffffff00'
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
    )
];

export default testSteps;