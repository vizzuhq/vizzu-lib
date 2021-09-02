import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country'] },
                    x: { attach: ['Joy factors', 'Value 5 (+/-)'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Joy factors'] },
                },
                title: 'Stacked Bar Chart',
                orientation: 'vertical',
                legend: null
            },
            style:
            {
                plot:
                {
                    paddingLeft: 120,
                    yAxis:
                    { 
                        label:
                        { 
                            paddingRight: 20, 
                            fontSize: 11 
                        }
                    },
                    marker:
                    {
                        borderWidth: 0
                    }
                }
            }
        }
    )
];

export default testSteps;