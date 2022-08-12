import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: ['Country', 'Value 2 (+)'] },
                    color: { set: 'Country' }
                },
                title: 'Percentage Stacked Area Chart',
                geometry: 'area',
                align: 'stretch',
            },
            style: {
                plot: {
                    paddingLeft: '1.2em',
                    yAxis: {
                        label: { paddingRight: '0.8em' }
                    },
                    xAxis: {
                        label: { paddingTop: '0.8em' }
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;