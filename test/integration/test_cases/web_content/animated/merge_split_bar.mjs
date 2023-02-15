import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            record.Country == 'Austria' ||
            record.Country == 'Belgium' ||
            record.Country == 'Bulgaria' ||
            record.Country == 'Cyprus' ||
            record.Country == 'Czechia' ||
            record.Country == 'Denmark'
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