import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
                    x: { attach: ['Joy factors', 'Country'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Grouped Column Chart with (-) Nums'

            },
            style:
            {
                data:
                { columnMaxPadding: 0.14 },
                plot:
                {
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