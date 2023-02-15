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
                y: ['Value 3 (+)', 'Country_code'],
                color: 'Country_code'
            },
            title: 'Stacked Area',
            geometry: 'area'
        }
    }),
    chart => chart.animate({
            data: { 
                filter: record => data.filter(record) 
                    && record.Year < 12 && record.Year > 6 
            },
            config: {
                title: 'Zoomed Stacked Area'
            }    
        })
];

export default testSteps;