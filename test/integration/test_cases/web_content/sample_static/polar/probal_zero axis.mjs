import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors'] },
                    x: { attach: ['Value 2 (+)'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Pie Chart',
                coordSystem: 'polar',
                geometry: 'rectangle'
 //               align: 'stretch'
            },
            style:
            {
                plot:
                { 
                    marker:
                    { 
                        label:
                        { 
                            fontSize: 14,                        
                            position: 'center'
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
                      yAxis:
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