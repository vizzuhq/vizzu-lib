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
        }),
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
            },
            title: 'Polar Stacked Area Chart',
            coordSystem:'polar',
            geometry: 'area',
            legend: null
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: ['Year', 'Value 2 (+)'],
                y: 'Country',
                color: 'Country',
            },
            title: 'Radial Bar Chart',
            geometry: 'rectangle',
//            orientation: 'horizontal',
            split: false,
        }
    },
    {
         geometry: { 
             delay: 0.7, 
             duration: 0.5
         },
         x: {
             delay: 1
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
        title: 'Polar Stacked Area Chart',
        geometry: 'area',
        legend: null
    } 
},
{
     geometry: { 
         delay: 0.7, 
         duration: 0.5
     },
     x: {
         delay: 1
     }
 }

)];

export default testSteps;