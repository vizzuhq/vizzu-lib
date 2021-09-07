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
                title: 'Pie Chart',
                coordSystem: 'polar'
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
                        },
                        interlacing: 
                        {
                            color: '#ffffff00'
                        },
                        ticks: 
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
                        },
                        ticks: 
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
                channels:
                {
                    y: { attach: ['index'], range: {min:'-200%', max:'100%'} }
                },
                title: 'Donut Chart'
            }
        }
    )
];

export default testSteps;