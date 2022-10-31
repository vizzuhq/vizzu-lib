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
                color: { set: 'Joy factors' },
                size: { set: 'Value 1 (+)' }
            },
            title: 'Line Chart'
        }
    }
    )
];

export default testSteps;