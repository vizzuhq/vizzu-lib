import { data } from '../../../../test_data/chart_types_eu.mjs';


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
            legend: null
        },
        style: {
            plot: {
                paddingLeft: 100,
                yAxis: {
                    label: {
                       paddingRight: 10,
                        fontSize: 13
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'stretch',
            split: false
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
                duration: 0, 
//                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 1,
                easing: 'ease-in-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
                easing: 'ease-in-out'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: 'Value 2 (+)' },
                color: { set: null }
            },
            title: 'Tile',
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
                y: { set: 'Value 2 (+)' },
                color: { set: null }
            },
            title: 'Tile',
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
                x: { set: ['Joy factors'] },
                y: { set: 'Value 2 (+)' },
                color: { set: null }
            },
            title: 'Column Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'none',
            split: false
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
                duration: 0, 
//                easing: 'linear' 
            },
            x: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(0.65,0,0.65,1)'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(0.65,0,0.65,1)'
            }
        }
    )];

export default testSteps;