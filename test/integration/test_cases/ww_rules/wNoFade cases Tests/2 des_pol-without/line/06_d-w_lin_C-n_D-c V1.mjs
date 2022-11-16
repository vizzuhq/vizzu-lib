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
            x: { set: null },
            y: { set: null },
            color: { set: 'Joy factors' },
            noop: { set: 'Year' },
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
//                easing: 'ease-out'
     }, 
     y: {
         delay: 0,
         duration: 0.75, // Rectangle=0.5
        }
    }
)];

export default testSteps;