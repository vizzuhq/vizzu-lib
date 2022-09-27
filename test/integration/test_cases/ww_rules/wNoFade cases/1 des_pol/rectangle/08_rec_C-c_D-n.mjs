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
                x: { set: ['Year','Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Mekko Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country','Year', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Column Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
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
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country','Year', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Column Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
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
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: 'Joy factors' }
            },
            title: 'Stacked Column Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
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
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors','Value 1 (+)'] },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Joy factors' }
            },
            title: 'Mekko Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
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
                delay: 0,
                duration: 0.5,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0.5,
                duration: 0.5,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    )];

export default testSteps;