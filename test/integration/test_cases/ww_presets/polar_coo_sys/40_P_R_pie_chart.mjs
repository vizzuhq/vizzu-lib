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
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Value 2 (+)' }
                },
                title: 'Pie Chart',
                coordSystem: 'polar'
            },
            style:
            {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    xAxis: {
                          title: { color: '#ffffff00' },
                          label: { color: '#ffffff00' },
                          interlacing: { color: '#ffffff00' }
                    },                 
                    yAxis: {
                        color: '#ffffff00',
                          title: { color: '#ffffff00' },
                          label: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;