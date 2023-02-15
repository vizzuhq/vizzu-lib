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
                x: ['Year', 'Joy factors'],
                y: 'Value 3 (+)',
                color: 'Country_code'
            },
            title: 'Polar Line chart',
            geometry: 'line',
            coordSystem: 'polar'
        }
    }),
    chart => chart.animate({
        data: { 
            filter: record => data.filter(record) 
                && record.Year < 8 && record.Year > 2 
        },
        config: {
            title: 'Zoomed Polar Line chart'
        }
    })
];

export default testSteps;