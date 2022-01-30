import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year', 'Value 5 (+/-)'] }
            },
            title: 'Dot Plot',
            geometry: 'circle'
        }
    })
];

export default testSteps;