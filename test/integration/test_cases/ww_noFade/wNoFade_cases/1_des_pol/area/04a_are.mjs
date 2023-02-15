import { data } from '../../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
      data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
               record.Country == 'Hungary'
        }),,
        config: {
            channels: {
                x: 'Country_code',
                y: ['Joy factors', 'Value 2 (+)'],
                color: 'Joy factors'
            },
            title: 'Stacked Area Chart',
            geometry: 'area',
            orientation: 'horizontal'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Country_code',
                y: 'Joy factors'
            },
            title: 'Remove Conti',
            align: 'stretch',
            orientation: 'horizontal'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: 'Joy factors'
            },
            title: 'Change Discrete',
            orientation: 'horizontal'
        }
    },
    {
             duration: 0.5
     }
    ),

    chart => chart.animate({
    config: {
        channels: {
            x: 'Year',
            y: ['Joy factors', 'Value 2 (+)']
        },
        title: 'Add previous Conti',
        align: 'min',
        orientation: 'horizontal'
    } 
})];

export default testSteps;