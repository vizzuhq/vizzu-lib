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
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
            },
            title: 'Stacked Column Chart',
            geometry: 'rectangle',
            coordSystem: 'polar',
            legend: null
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: ['Value 2 (+)', 'Year'],
                y: 'Country',
                color: 'Country',
            },
            title: 'Stacked Area Chart',
            geometry: 'area',
            orientation: 'horizontal',
            split: false,
        }
    }
    ),

    
    chart => chart.animate({
    config: {
        channels: {
            x: 'Year',
            y: ['Country', 'Value 2 (+)'],
            color: 'Country',
        },
        title: 'Stacked Column Chart',
        geometry: 'rectangle',
        legend: null
    } 
}

)];

export default testSteps;