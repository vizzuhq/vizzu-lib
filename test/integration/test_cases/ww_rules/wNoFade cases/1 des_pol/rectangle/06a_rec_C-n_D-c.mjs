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
            title: 'Stacked Column Chart'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar, Remove Conti % Change Orient.',
            orientation: 'vertical'
        }
    },
    {
     easing: 'cubic-bezier(0.65,0,0.65,1)',
         x: {
             delay: 0,
             duration: 0.75
         }, 
         y: {
             delay: 0,
             duration: 0.75
         }
     }
    ),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: 'Country' },
            color: { set: 'Country' }
        },
        title: 'Bar Change Disc'
    } 
},
       {
                duration: 0
        }

),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: ['Country', 'Value 4 (+/-)'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Column Chart Add Conti & Chenge Orient.',
        orientation: 'horizontal'
    } 
},
{
 easing: 'cubic-bezier(0.65,0,0.65,1)',
     x: {
         delay: 0,
         duration: 0.75
     }, 
     y: {
         delay: 0,
         duration: 0.75
     }
 }
)];

export default testSteps;