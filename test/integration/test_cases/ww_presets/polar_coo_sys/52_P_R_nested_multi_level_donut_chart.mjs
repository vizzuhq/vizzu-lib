import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' 
            }),
            config:
            {
                channels:
                {
                    x: { attach: ['Joy factors', 'Value 2 (+)'] },
                    y: { attach: 'Country', range: { min: '-50%' }  },
                    color: { attach: 'Joy factors' },
                    label: { attach: 'Value 2 (+)' },
                },
                title: 'Nested (Multi-level) Donut Chart',
                align: 'stretch',
                coordSystem: 'polar'
            },
            style: {
                plot: {
                    paddingLeft: '0em',
                    paddingRight: '12.42em',
                    marker: {
                        rectangleSpacing:'0',
                        borderWidth: 1,
                        borderOpacity: 0
                        },
                    yAxis: {
                        color: '#ffffff00',
                        title: { color: '#ffffff00' },
                        label: { color: '#ffffff00' },
                        ticks: { color: '#ffffff00' }
                    },
                    xAxis: {
                        title: { paddingTop: '2.4em' },
                        interlacing: { color: '#ffffff00' },
                        label: { color: '#ffffff00' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;