import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Joy factors', 'Value 2 (+)'], range: '0,1.075,%' },
                    x: { attach: ['Year'] },
                    color: { attach: ['Joy factors'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar',
                legend: null
            },
            style: 
            {
                legend:{backgroundColor: 'rgb(180,180,180 )' 
            },
                data: 
                { 
                    columnMaxPadding: 0.21 
                },
                plot: 
                { 
                    marker: 
                    {
                        borderOpacity:1
                    } 
                }
            }
        }
    )
];

export default testSteps;