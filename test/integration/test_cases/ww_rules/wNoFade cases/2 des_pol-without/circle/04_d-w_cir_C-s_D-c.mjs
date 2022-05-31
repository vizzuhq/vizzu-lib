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
                y: { set: 'Value 2 (+)' },
                color: { set: 'Country' }
            },
            title: 'Lollipop Chart',
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
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                size: { set: 'Country' }
            },
            title: 'Lollipop Chart',
            geometry: 'circle',
//            orientation: 'horizontal'
        }
    },
       {
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
                duration: 0,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Joy factors' },
                size: { set: 'Joy factors' }
            },
            title: 'Lollipop Chart',
            geometry: 'circle',
//            orientation: 'horizontal'
        }
    },
       {
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
                duration: 0,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Joy factors' },
                size: { set: null }
            },
            title: 'Lollipop Chart',
            geometry: 'circle',
//            orientation: 'horizontal'
        }
    },
       {
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
                duration: 0,
//                easing: 'ease-out'
            }, 
            y: {
                delay: 0,
                duration: 1,
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                noop: { set: 'Year' },
                size: { set: ['Year', 'Value 3 (+)'] },
                color: { set: 'Joy factors' }
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
    //                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    )
];

export default testSteps;