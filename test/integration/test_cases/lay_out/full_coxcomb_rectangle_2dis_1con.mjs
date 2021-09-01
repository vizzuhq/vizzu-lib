import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'], range: '0,1.075,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar'
            },
            style: 
            {
                title: {backgroundColor: 'rgb(200,200,200 )'},
                legend:{backgroundColor: 'rgb(230,230,230 )'},
                plot: 
                { 
                    backgroundColor: 'rgb(215,215,215 )',
                    marker: 
                    {
                        borderOpacity:1
                    } 
                },
                data: 
                {
                    columnMaxPadding: 0.21 
                }
            }
        }
    )
];

export default testSteps;