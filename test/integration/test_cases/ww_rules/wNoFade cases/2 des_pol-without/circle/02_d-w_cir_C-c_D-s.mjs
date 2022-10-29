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
                noop: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Country' }
            },
            title: 'Distribution',
            geometry: 'circle',
            legend: null
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                noop: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                lightness: { set: 'Value 3 (+)' },
                color: { set: 'Country' }
            },
            title: 'Bubble Chart',
            geometry: 'circle',
            orientation: 'horizontal'
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
                duration: 1, 
//                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 1,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                noop: { set: 'Year' },
                lightness: { set: 'Value 3 (+)' },
                size: { set: 'Value 2 (+)' },
                color: { set: 'Country' }
            },
            title: 'Bubble Chart',
            geometry: 'circle',
            orientation: 'horizontal'
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
                duration: 1, 
//                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 1,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    )];

export default testSteps;