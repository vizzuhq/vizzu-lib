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
                x: { set: ['Year','Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Mekko Chart',
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
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar',
            geometry: 'rectangle',
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
            x: { set: ['Joy factors', 'Value 3 (+)'] },
            y: { set: 'Country' },
            color: { set: 'Country' }
        },
        title: 'Bar',
        geometry: 'rectangle',
        orientation: 'vertical',
        align: 'none',
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
                duration: 0
        }

),
chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Joy factors', 'Value 3 (+)'] },
            y: { set: ['Country', 'Value 1 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Mekko Chart',
        geometry: 'rectangle',
        orientation: 'horizontal',
        align: 'none',
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

)];

export default testSteps;