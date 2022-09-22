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
            legend: null
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar',
            geometry: 'rectangle',
            orientation: 'vertical',
            split: false
        }
    },
       {
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            geometry: { 
                delay: 0.5, 
                duration: 0.5, 
//                easing: 'linear' 
            },
            x: {
                delay: 0.5,
                duration: 0.5,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 0.5,
//                easing: 'cubic-bezier(65,0,65,1)'
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
        title: 'Bar',
        geometry: 'rectangle',
        orientation: 'vertical',
        legend: null
    } 
},
       {
                duration: 0
        }

),
/*
chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: ['Country', 'Value 1 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Column Chart (negative sum)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        legend: null
    } 
},
       {
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            geometry: { 
                delay: 0, 
                duration: 0.5, 
       //                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 0.5,
       //                easing: 'ease-out'
            }, 
            y: {
                delay: 0.5,
                duration: 0.5,
       //                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
),
*/
chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },
            color: { set: 'Country' },
            noop: { set: 'Joy factors' },
            size: { set: ['Joy factors', 'Value 1 (+)'] }
        },
        title: 'Treemap',
        geometry: 'rectangle',
        orientation: 'horizontal',
        align: 'none',
        legend: null
    } 
},
       {
        easing: 'cubic-bezier(0.65,0,0.65,1)',
        delay:1,
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            geometry: { 
                delay: 0, 
                duration: 0.5, 
       //                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 0.5,
       //                easing: 'ease-out'
            }, 
            y: {
                delay: 0.5,
                duration: 0.5,
       //                easing: 'cubic-bezier(65,0,65,1)'
            }
        }

)
];

export default testSteps;