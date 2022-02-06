import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    x: { attach: ['Joy factors', 'Country'] },
                    color: { attach: ['Joy factors'] },
                    label: { attach: ['Value 5 (+/-)'] }
                },
                title: 'Grouped Column Chart with (-) Nums'

            },
            style:
            {
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