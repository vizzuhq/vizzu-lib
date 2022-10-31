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
        }),
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 3 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area Chart'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                color: { set: 'Country' },
                noop: { set: 'Year' },
                size: { set: ['Year', 'Value 3 (+)'] }
            },
            title: 'Bubble',
            geometry: 'circle'
        }
    },
    {
     easing: 'cubic-bezier(0.65,0,0.65,1)',
     coordSystem: { 
             delay: 0,
             duration: 1,
         },
         geometry: { 
             delay: 0, 
             duration: 0.5
         },
         x: {
             delay: 0.25,
             duration: 0.75
         }, 
         y: {
             delay: 0,
             duration: 1
         }
     }
    )];

export default testSteps;