import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data_6,
        config: {
            channels: {
                x: 'Year',
                y: 'Value 3 (+)',
                color: 'Country_code'
            },
            title:'Title',
            geometry: 'line'
        }
    }),
    chart => chart.animate({
        data: { 
            filter: record => data_6.filter(record) 
            && record.Year < 14 && record.Year > 8 
        },
        config: {
            title:'Title'
        }
    }),
    chart => chart.feature('tooltip',true)
];

export default testSteps;