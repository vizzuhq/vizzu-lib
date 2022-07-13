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
                x: { set: null },
                y: { set: ['Country','Year', 'Value 2 (+)'], range: { min: '0%', max: '100%' }  },
                color: { set: 'Country' }
            },
            title: '1 Stacked Column Chart',
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
//                easing: 'cubic-bezier(65,0,65,1)'
            }
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        },
        style: {
            plot: {
                marker: { rectangleSpacing: 0 }
            }
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
                x: { set: null },
// circlenél ide kell                noop: { set: 'Joy factors' }
            },
            title: 'Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        },
        style: {
            plot: {
                marker: { rectangleSpacing: 0 }
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
                x: { set: null },
                y: { set: null },
                noop: { set: 'Joy factors' },
                size: { set: 'Value 2 (+)' },
                lightness: { set: 'Value 1 (+)' }
            },
            title: 'Treemap',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        },
        style: {
            plot: {
                marker: { rectangleSpacing: 0 }
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
    ),


    chart => chart.animate({
        config: {
            channels: {
                color: { set: 'Joy factors' }
            },
            title: 'Treemap',
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