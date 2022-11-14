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
            title: 'Stacked Column Chart',
            geometry: 'rectangle'
        } 
}),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar1 Remove Conti, Change Orientation',
            orientation: 'vertical'
        }
        ,
        style: {
            plot: {
                marker: { rectangleSpacing: '0' }
            }
        }
},
{
    easing: 'cubic-bezier(.39,0,.35,.99)'
}),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Joy factors' },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar2 Change Discrete'
        }
    },
    {
        delay: 0,      
        duration: 0
}),
   
    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Bar3 Change Orientation',
        orientation: 'horizontal'
    },
    style: {
        plot: {
            marker: { rectangleSpacing: null }
        }
    }
},
{
    duration: 1,
    easing: 'cubic-bezier(.39,0,.35,.99)',
     x: {
         delay: 0,
         duration: 1
     }, 
     y: {
         delay: 0,
         duration: 1
     }
})
];

export default testSteps;