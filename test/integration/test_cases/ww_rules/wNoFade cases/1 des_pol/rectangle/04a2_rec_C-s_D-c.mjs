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
            geometry: 'rectangle',
            orientation: 'horizontal',
//            legend: null
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 2 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Bar1',
            orientation: 'vertical',
            align: 'stretch'
        },
        style: {
            plot: {
                marker: { rectangleSpacing: '0' }
            }
        }
    },
    {   
        duration: 1,
        easing: 'cubic-bezier(.39,0,.35,.99)',
        style: {
            delay: 0,
            duration: 0.5,
        },
        x: {
            delay: 0.5,
            duration: 0.5
        }, 
        y: {
            delay: 0,
            duration: 0.5
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 2 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Bar2'
        }
    },
    {
        delay: 0,      
        duration: 0
     }
    ),
   
    chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Joy factors', 'Value 2 (+)'] },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Bar3',
        orientation: 'horizontal',
        align: 'stretch'
    }
},
       {
        delay: 0,        
        duration: 2,
        x: {
            delay: 0,
            duration: 1
        }, 
        y: {
            delay: 1,
            duration: 1
        }
    } 
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Column Chart',
        align: 'none'
    },
    style: {
        plot: {
            marker: { rectangleSpacing: null }
        }
    }
},
{
     x: {
         delay: 0,
         duration: 1,
         easing: 'cubic-bezier(.39,0,.35,.99)'
     }, 
     y: {
         delay: 0,
         duration: 1,
         easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }

)];

export default testSteps;