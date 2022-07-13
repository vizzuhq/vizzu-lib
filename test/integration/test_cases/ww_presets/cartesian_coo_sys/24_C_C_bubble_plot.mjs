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
                paddingLeft: '1.2em',
                yAxis: {
                label: { paddingRight: '0.8em' }
                },
            xAxis: {
                title: { paddingTop: '2.4em' },
                label: { paddingTop: '0.8em' }
                }
            }
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;