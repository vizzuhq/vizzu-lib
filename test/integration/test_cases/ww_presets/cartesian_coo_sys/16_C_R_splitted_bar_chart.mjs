import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Value 2 (+)', 'Joy factors'] },
                    y: { attach: 'Year' },
                    color: { attach: 'Joy factors' }
                },
                title: 'Splitted Bar Chart',
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
                        label: { paddingTop: 10 },
                        title: { paddingTop: 30 }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;