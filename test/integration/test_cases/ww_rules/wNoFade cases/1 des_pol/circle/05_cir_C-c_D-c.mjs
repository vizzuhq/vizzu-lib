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
        config:{
            channels:
            {
                y: { set: 'Value 6 (+/-)' },
                x: { set: 'Value 2 (+)' },
                color: { set: 'Country' },
                noop: { set: 'Joy factors' }
            },
            title: 'Scatter plot',
            geometry: 'circle'
        }
    }
),
chart => chart.animate({
        config:{
            channels:
            {
                y: { set: 'Value 3 (+)' },
                x: { set: 'Value 2 (+)' },
                color: { set: 'Country' },
                noop: { set: 'Joy factors' }
           },
            title: 'Change Continuous',
            geometry: 'circle'
        }
    }
),

chart => chart.animate({
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Change Discrete'
        }
    }
),

chart => chart.animate({
        config: {
            channels:
            {
                noop: { set: null },
                size: { set: ['Year'] }
            },
            title: 'Change Discrete'
        }
    },
    {
             duration: 0
     }
),

chart => chart.animate({
        config: {
            channels:
            {
                noop: { set: 'Year' },
                size: { set: null }
            },
            title: 'Scatter plot.'
        }
    }
)
];

export default testSteps;