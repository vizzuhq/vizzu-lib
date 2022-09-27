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
            geometry: 'line',
            orientation: 'horizontal',
            legend: null
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
            title: 'Line Chart',
            geometry: 'line'
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

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Line Chart',
            geometry: 'line'
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
            y: { set: ['Value 2 (+)'] },
            color: { set: 'Joy factors' },
            size: { set: null }
        },
        title: 'Line Chart',
        geometry: 'line'
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
        geometry: 'circle',
        orientation: 'horizontal'
    }
},
   {
     delay: 1,
     easing: 'cubic-bezier(0.65,0,0.65,1)',
     coordSystem: {
         delay: 0, // Rectangle=0.5
         duration: 1, // Rectangle=0.5
     },
     geometry: { 
         delay: 0.25, 
         duration: 0.75,  // Rectangle=0.5
     },
     x: {
         delay: 0.25, // Rectangle=0.5
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