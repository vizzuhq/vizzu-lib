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
                    y: { title: 'PRÓBA' },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Pie Chart',
                geometry: 'rectangle',
                coordSystem: 'polar',
                align: 'stretch'
            },
            style:
            {
                plot:
                { 
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
                        rectangleSpacing: 0,
                        label:
                        { 
                            fontSize: 14,                        
                            position: 'center'

                        }
                    }
                }
            }
        }
    )
];

export default testSteps;