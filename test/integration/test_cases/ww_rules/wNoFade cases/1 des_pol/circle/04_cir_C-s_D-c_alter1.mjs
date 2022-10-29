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
        config:
        {
            channels:
            {
                y: { set: 'Value 4 (+/-)' },
                x: { set: 'Value 2 (+)' },
                color: { set: 'Country' },
                noop: { set: 'Year' },
                size: { set: 'Value 3 (+)' }
            },
            title: 'Dot plot',
            geometry: 'circle',
            legend: null
        }
    }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: ['Value 3 (+)', 'Year'] }
            },
            title: 'Dot plot (stacked).'
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
             duration: 1,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0,
             duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
         }
     }
),

chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: ['Value 3 (+)', 'Year', 'Joy factors'] }
            },
            title: 'Dot plot (stacked).'
        }
    },
    {
             duration: 0.5
     }
),


chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: ['Value 3 (+)', 'Joy factors'] }
            },
            title: 'Dot plot (stacked).'
        }
    },
    {
             duration: 0.5
     }
),



chart => chart.animate(
    {
        config: {
            channels:
            {
                noop: { set: 'Joy factors' },
                size: { set: 'Value 3 (+)' }
            },
            title: 'Dot plot'
        }
    }
)
];

export default testSteps;