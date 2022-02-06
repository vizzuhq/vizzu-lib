import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data,
        config: {
            channels: {
                x: { set: ['Value 5 (+/-)'] },
                y: { set: ['Country'] },
                color: { set: ['Country'] },
                label: { set: ['Value 5 (+/-)'] }
            },
            title: 'Bar Chart',
            orientation: 'vertical'
        }
    })
];

export default testSteps;