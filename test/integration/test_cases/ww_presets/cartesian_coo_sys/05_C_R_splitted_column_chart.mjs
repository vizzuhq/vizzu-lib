import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { attach: ['Value 2 (+)', 'Joy factors'] },
                    x: { attach: 'Year' },
                    color: { attach: 'Joy factors' },
//                    label: { attach: 'Value 2 (+)' }
                },
                title: 'Splitted Column Chart',
                split: true
            },
            style: {
                plot: {
                    paddingLeft: 0,
                    yAxis: {
                        label: {
                           paddingRight: 10
                        }
                    },
                    xAxis: {
                        label: { paddingTop: 10 }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;