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
                    y: { attach: ['index'], range: {min:'-200%', max:'100%'} },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Donut Chart',
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
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;