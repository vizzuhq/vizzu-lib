import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Country'] },
                    x: { attach: ['Threat', 'Negative nums'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    lightness: { attach: ['Threat'] },
                },
                title: 'Stacked Bar Chart',
                orientation: 'vertical',
                legend: null
            },
            style:
            {
                data:
                { 
                    barMaxPadding: 0.16 
                },
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