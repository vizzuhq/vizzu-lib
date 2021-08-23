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
                    x: { attach: ['Negative nums'], range: '0,1.1,%' },
                    color: { attach: ['Country'] },
                    label: { attach: ['Negative nums'] },
                },
                title: 'Bar Chart with (-) Nums',
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
                    { label:
                        {
                            fontSize: 10
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;