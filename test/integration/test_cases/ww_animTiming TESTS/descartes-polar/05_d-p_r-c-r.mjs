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
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)', 'Country'] },
                color: { set: ['Country'] },
            },
            title: 'Stacked Column Chart',
            geometry: 'rectangle',
            legend: null
        }
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Value 4 (+/-)'] },
                y: { set: ['Value 2 (+)'] },
                noop: { set: ['Year'] },
                color: { set: ['Country'] },
            },
            title: 'Spider Dotplot Fake',
            geometry: 'circle',
            coordSystem: 'polar',
            split: false,
        }
    },
       {
        duration: 2.5,
        easing: 'cubic-bezier(.39,0,.35,.99)', // 'cubic-bezier(.39,0,.35,.99)',
            coordSystem: {
                delay: 0,
                duration: 2.5,
            },
            title: {
                delay: 0,
                duration: 2.5,
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
                easing: 'cubic-bezier(.39,0,.35,.99)',
                delay: 0.25,
                duration: 0.75
            }
        }
    ),

    
    chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Year'] },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: ['Country'] },
        },
        title: 'Stacked Column Chart',
        geometry: 'rectangle',
        coordSystem: 'cartesian',
        orientation: 'horizontal',
        legend: null
    } 
},
       {
        delay: 0,
        duration: 1.5,
        easing: 'cubic-bezier(.39,0,.35,.99)', // 'cubic-bezier(.39,0,.35,.99)',
            coordSystem: {
                delay: 0,
                duration: 1.5,
            },
            geometry: { 
                delay: 0.5, 
                duration: 0.5
            },
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