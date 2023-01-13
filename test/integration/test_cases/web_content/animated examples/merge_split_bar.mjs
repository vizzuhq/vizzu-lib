import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            [ 'AT', 'BE', 'DE', 'DK', 'ES' ]
            .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: ['Value 3 (+)', 'Country'],
                y: ['Year', 'Joy factors'],
                color: 'Country'
            },
            title: 'Stacked Bar Chart'
        }
    }),

    chart => chart.animate({
        config: {
            title: 'Split Bar Chart',
            split: true
        }
    })
];

export default testSteps;