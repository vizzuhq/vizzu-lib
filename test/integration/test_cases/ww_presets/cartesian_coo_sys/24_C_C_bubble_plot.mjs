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
                size: { set: 'Value 3 (+)' },
                label: { set: 'Country' }
            },
            title: 'Bubble Plot',
            geometry: 'circle'
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;