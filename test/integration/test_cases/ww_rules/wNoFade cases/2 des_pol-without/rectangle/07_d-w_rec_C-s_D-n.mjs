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
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Mekko Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        } 
    }),

    // stretch, color levesz
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: '1 Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'stretch',
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
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),
// 2 disc levesz
    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Value 3 (+)' },
                y: { set: 'Value 2 (+)', range: { min: '0%', max: '100%' }  }
            },
            title: '2 Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        }
    },
       {
                duration: 0
        }
    ),
// disc ratesz
    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors', 'Value 3 (+)'] }
            },
            title: '3 Tile',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        }
    },
       {
                duration: 0
        }
    ),

/*
    // Vissza column chartba
    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Joy factors' },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Joy factors' }
            },
            title: 'Column Chart',
            geometry: 'rectangle',
            orientation: 'horizontal',
            align: 'none',
            split: false
        },
        style: {
            plot: {
                marker: { rectangleSpacing: null }
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
                duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
            }
        }
    ),
*/

// coordSys valtas (nem latszik)
chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: 'Value 2 (+)' }
        },
        title: '4 Tile',
        geometry: 'rectangle',
        orientation: 'horizontal',
        align: 'none',
        split: false
    }
},
{
         duration: 0
 }
),

// Treemap megcsinal
    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                noop: { set: 'Joy factors' },
                lightness: { set: 'Value 3 (+)' },
                size: { set: 'Value 2 (+)' }
            },
            title: 'Treemap',
            geometry: 'rectangle',
            orientation: 'horizontal',
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
    ),
 

    // color ratesz
    chart => chart.animate({
        config: {
            channels: {
                color: { set: 'Joy factors' }
            },
            title: 'Treemap',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        },
        style: {
            plot: {
                marker: { rectangleSpacing: null }
            }
        }
    },
    {
        duration: 1
    }
    )


];

export default testSteps;