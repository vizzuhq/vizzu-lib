import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat', 'Positive nums'] },
                    x: { attach: ['Year', 'Positive nums'] },
                    color: { attach: ['Threat'] }
                },
                title: 'Stacked Coxcomb Chart',
                coordSystem: 'polar',
                legend: 'color'
            },
            style: 
            {
                plot: 
                { 
                yAxis:                    
                { title:
                    {
                        paddingBottom: '25' 
                    }
                },                
                xAxis:                    
                { title:
                    {
                        side: 'positive',
                        paddingBottom: '25' 
                    }
                },
                    marker: 
                    { 
                        borderWidth:0.7,
                        borderOpacity:1
                    } 
                }
            }
        }
    )
];

export default testSteps;