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
                title: 'Stacked Area Chart',
                geometry: 'area'
            },
            style: {
                plot: {
                    paddingLeft: 0,
                    yAxis: {
                        label: { paddingRight: 10 }
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