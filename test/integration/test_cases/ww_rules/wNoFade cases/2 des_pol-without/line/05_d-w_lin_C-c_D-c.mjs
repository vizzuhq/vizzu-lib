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
                color: { set: 'Country' },
                size: { set: 'Value 1 (+)' }
            },
            title: 'Line Chart',
            geometry: 'line'
        },
        style: {
            plot: {
                marker: { lineMaxWidth: 0.02 }
            }
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                size: { set: ['Country', 'Value 1 (+)'] }
            },
            title: 'Stack Line'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: null  },
                size: { set: ['Country', 'Value 1 (+)'] }
            },
            title: 'Change Conti'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: null },
                size: { set: ['Joy factors', 'Value 1 (+)'] }
            },
            title: 'Change Stack Disc'
        }
    },
       {
                duration: 0
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: 'Joy factors' },
                size: { set: 'Value 1 (+)' }
            },
            title: 'Group Line Chart'
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
                size: { set: ['Year', 'Value 1 (+)'] }
            },
            title: 'Bubble Chart (stacked)',
            geometry: 'circle'
        }
    },
       {
         easing: 'cubic-bezier(0.65,0,0.65,1)',
         geometry: { 
             delay: 0, 
             duration: 0.75,  // Rectangle=0.5
         },
         x: {
             delay: 0.5, // Rectangle=0.5
             duration: 0.75, // Rectangle=0.5
         }, 
         y: {
             delay: 0,
             duration: 0.75, // Rectangle=0.5
            }
        }
    )

];

export default testSteps;