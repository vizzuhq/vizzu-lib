import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data_8,
        config: {
            channels: {
                x: 'Year',
                y: ['Value 2 (+)', 'Country_code'],
                color: 'Country_code'
            },
            title:'Title',
            geometry: 'area',
            coordSystem: 'polar'
        }
    }),

    chart => chart.animate(
        {
            data: {
                filter: record =>
                    record['Country_code'] == 'FR' ||
                    record['Country_code'] == 'CY' 
                },
            config: {
                title:'Title'
            } 
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;