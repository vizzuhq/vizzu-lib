import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Threat'] },
                    x: { attach: ['Positive nums'], range: '0,1.33,%' },
                    color: { attach: ['Threat'] }
                },
                title: 'Radial Chart',
                coordSystem: 'polar',
                legend: null
            },
            style:
            {
                data:
                {
                    barMaxPadding: 0.25
                },
                plot:
                { 
                    xAxis:
                    { 
                        title:
                        {
                            side: 'positive',
                            paddingTop: '25' 
                        }
                    
                    },
                  yAxis:
                    { 
                        label:
                        { 
                            paddingRight: 20
                        }
                    
                    }
                }
            }
        }
    )
];

export default testSteps;