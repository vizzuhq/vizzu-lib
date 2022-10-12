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
            x: { set: ['Year','Value 3 (+)'] },
            y: { set: ['Country', 'Value 1 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Mekko Chart (exchange conti)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        legend: null
    } 
}),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Year','Value 3 (+)'] },
            y: { set: ['Country', 'Value 1 (+)'] },
            color: { set: null }
        },
        title: 'Mekko Chart (- Disc)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        align: 'stretch',
        legend: null
    } 
}),



    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 3 (+)'] },
                y: { set: 'Value 1 (+)' },
                color: { set: null }
            },
            title: 'Mekko Chart (+ Disc)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'stretch',
            split: false
        }
    },
       {
            duration: 0
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 3 (+)'] },
                y: { set: 'Value 1 (+)' },
                color: { set: 'Joy factors' }
            },
            title: 'Mekko Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'min',
            split: false
        }
    },
       {
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            x: {
                delay: 0.5,
                duration: 0.5,
            }, 
            y: {
                delay: 0,
                duration: 1,
            }
        }
    )
];

export default testSteps;
