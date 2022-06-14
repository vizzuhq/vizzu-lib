import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: 'Value 4 (+/-)' },
                y: { set: 'Value 5 (+/-)' },
                noop: { set: 'Country' },
                color: { set: 'Joy factors' },
                size: { set: 'Value 3 (+)' }
            },
            title: 'Bubble Plot',
            geometry: 'circle'
        },
        style:
        {
            plot: {
                paddingLeft: 0,
            yAxis: {
                label: { paddingRight: 10 }
                },
            xAxis: {
                title: { paddingTop: 30 },
                label: { paddingTop: 10 }
                }
            }
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;