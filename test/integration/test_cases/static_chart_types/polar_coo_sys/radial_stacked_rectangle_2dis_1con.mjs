import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Year'] },
                    x: { attach: ['Country', 'Positive nums'], range: '0,1.35,%' },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Year'] }
                },
                title: 'Stacked Radial Chart',
                legend: 'color',
                coordSystem: 'polar'
            },
            style: {
                data: 
                { 
                    barMaxPadding: 0.13 
                },
                plot:
                { 
                  yAxis:
                    { 
                        label:
                        { 
                            fontSize: 8
                        }
                    
                    },
                    marker: 
                    {
                        borderOpacity: 1 
                    } 
                }
            }
        }
    )
];

export default testSteps;