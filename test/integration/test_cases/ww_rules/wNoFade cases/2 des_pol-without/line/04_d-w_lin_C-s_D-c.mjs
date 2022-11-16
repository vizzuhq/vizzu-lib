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
            title: 'Line Chart',
            geometry: 'line'
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
            title: 'Change Conti'
        }
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Change Stacked Disc'
        }
    },
    {
        delay: 0,
        duration: 0
         }
    ),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Value 2 (+)'] },
            color: { set: 'Joy factors' },
            size: { set: null }
        },
        title: 'Change Group Disc'
    } 
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },
            noop: { set: 'Year' },
            size: { set: ['Year', 'Value 2 (+)'] }
        },
        title: 'Bubble Chart (stacked)',
        geometry: 'circle'
    }
},
   {
     geometry: { 
         delay: 0, 
         duration: 0.75,  // Rectangle=0.5
     },
     x: {
         delay: 0.5, // Rectangle=0.5
         duration: 0.75, // Rectangle=0.5
//                easing: 'ease-out'
     }, 
     y: {
         delay: 0,
         duration: 0.75, // Rectangle=0.5
        }
    }
)];

export default testSteps;