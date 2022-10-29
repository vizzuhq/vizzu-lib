import { data } from '../../../test_data/correlogram_data.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: 'Alphabet 1' },
                y: { set: 'Alphabet 2', range: {max:'110%'} },
                color: { set: 'Frequency' },
                size: { set: 'Frequency' }
            },
            title: 'Correlogram',
            geometry: 'circle'
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;