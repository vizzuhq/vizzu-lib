import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    x: { attach: ['Joy factors', 'Value 2 (+)'] },
                    y: { attach: 'Value 3 (+)' },
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Value 2 (+)' }
                },
                title: 'Variable Radius Pie Chart',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    paddingLeft:'0',
                    paddingRight:'150',
                    xAxis: {
                        title: { color: '#ffffff00' },
                        label: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' }
                    },
                    yAxis:
                    {
                        color: '#ffffff00',
                        label: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;