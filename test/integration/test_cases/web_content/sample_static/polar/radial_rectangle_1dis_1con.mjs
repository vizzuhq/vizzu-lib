import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { set: ['Joy factors'], range: { min: '-30%', max: '100%' } },
                    x: { set: ['Value 2 (+)'], range: { min: '0%', max: '133%' } },
                    color: { set: ['Joy factors'] },
                    label: { set: ['Value 2 (+)'] }
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
                        color: '#ffffff00',
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