import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            descriptor:
            {
                channels:
                {
                    y: { attach: ['Joy factors','Value 2 (+)'], range: '0,1.1,%' },
                    x: { attach: ['Country'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 2 (+)'] }
                },
                title: 'Stacked Column Chart'

            },
            style:
            {
                data:
                { 
                    columnMaxPadding: 0.17 
                },
                plot:
                {
                    xAxis:
                    { label:
                        {
                            angle: 3.14*-3.25,
                            fontSize: 11
                        }
                    },
                    marker:
                    { label:
                        {
                            fontSize: 7,
                            orientation:'vertical',
                            angle:3.14*-1
                        }
                    }
                }
            }
        }
    )
];

export default testSteps;