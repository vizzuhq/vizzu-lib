import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Country'] },
                    x: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
                    color: { attach: ['Country'] },
                    label: { attach: ['Value 5 (+/-)'] },
                },
                title: 'Bar Chart with (-) Nums',
                orientation: 'vertical'
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