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
            orientation: 'horizontal'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Year', 'Country', 'Value 2 (+)']  },
                color: { set: null }
            },
            title: '1 Stacked Column Chart'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Tile Change Discs'
        }
        /*,
        style: {
            plot: {
                marker: { rectangleSpacing: 0 }
            }
        }
        */
    },
    {
        duration: 0
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                y: { set: null },
                color: { set: 'Joy factors' },
                size: { set: 'Value 2 (+)' },
                lightness: { set: 'Value 1 (+)' }
            },
            title: 'Treemap'
        }
    },
    {
     easing: 'cubic-bezier(0.65,0,0.65,1)',
         x: {
             delay: 0,
             duration: 0.75
         }, 
         y: {
             delay: 0.5,
             duration: 0.75
         }
     }
    )];

export default testSteps;