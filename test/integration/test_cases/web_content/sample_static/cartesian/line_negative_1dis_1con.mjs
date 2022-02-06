import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Year'] },
                y: { set: ['Value 5 (+/-)'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    })
];

export default testSteps;