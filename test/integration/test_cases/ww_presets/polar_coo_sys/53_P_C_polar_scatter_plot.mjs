import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: 'Value 3 (+)' },
                    y: { attach: 'Value 2 (+)', range:{max: '110%'} },
                    noop: { attach: ['Country'] }
                },
                title: 'Polar Scatter Plot',
                geometry: 'circle',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    paddingLeft: 50,
                    marker: { guides: {
                            color: null,
                            lineWidth: 0 }
                    },
                    yAxis: {
                        label: {
                           paddingRight: 0
                        },
                        color: '#ffffff00',
                        ticks: { color: '#ffffff00' }
                    },
                    xAxis: {
                      title: { paddingTop: 30 },
                      label: { paddingTop: 10 }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;