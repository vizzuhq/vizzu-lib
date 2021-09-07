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
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Donut Chart',
                coordSystem: 'polar'
            },
            style:
            {
                plot:
                { marker:
                    {   rectangleSpacing: 2.5,
                        label:
                        {
                            position: 'top' 
                        }
                    },                 
                    xAxis:
                      {
                        color: '#ffffff00',
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
                      },                 
                      yAxis:
                        {
                          color: '#ffffff00',
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

    )
];

export default testSteps;