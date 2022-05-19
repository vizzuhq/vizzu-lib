import { data } from '/test/integration/test_data/chart_types_eu.js';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Germany' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
               record.Country == 'Hungary'
        }),
        config: {
            channels: {
                size: { set: ['Value 2 (+)'] },
                color: { set: ['Country'] },
                noop: { set: ['Year'] },
            },
            title: 'Bubble Chart',
            geometry: 'circle',
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
                size: { set: null },
                x: { set: ['Year'] },
                y: { set: ['Value 2 (+)'] },
                color: { set: ['Country'] }
            },
            title: 'Line Chart',
            geometry: 'line',
//            orientation: 'horizontal',
            split: false,
        }
    },
       {
            coordSystem: {
                delay: 0,
                duration: 1,
            },
            geometry: { 
                delay: 0, 
                duration: 0.75, 
//                easing: 'linear' 
            },
            x: {
                delay: 0.25,
                duration: 0.75,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0.25,
                duration: 0.75,
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),
    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                size: { set: ['Year', 'Value 2 (+)'] },
                color: { set: ['Country'] },
                noop: { set: ['Year'] },
            },
            title: 'Bubble Chart',
            geometry: 'circle',
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
    },
    {
         coordSystem: {
             delay: 0,
             duration: 1,
         },
         geometry: { 
             delay: 0.25, 
             duration: 0.75, 
//                easing: 'linear' 
         },
         x: {
             delay: 0.25,
             duration: 0.75,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0,
             duration: 0.75,
//                easing: 'cubic-bezier(65,0,65,1)'
         }
     }
    ),
];

export default testSteps;