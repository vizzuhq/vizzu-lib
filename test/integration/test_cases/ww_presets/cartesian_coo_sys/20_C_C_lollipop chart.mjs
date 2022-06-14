import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: 'Year' },
                    y: { attach: 'Value 1 (+)' },
                    label: { attach: 'Value 1 (+)' }
                },
                title: 'Lollipop Chart',
                align: 'none',
                geometry: 'circle'
            },
            style:
            {
                plot: {
                    paddingLeft: 70,
                yAxis: {
                    label: { paddingRight: 10 }
                    },
                xAxis: {
                    title: { paddingTop: 30 },
                    label: { paddingTop: 10 }
                    },
                marker:
                    { label:
                        {                      
                            position: 'top',
                            maxFractionDigits: '1.0'
                        }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;