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
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stacked Area Chart',
            geometry: 'area',
            orientation: 'horizontal'
        } 
    }),

    chart => chart.animate({
            config: {
            channels: {
                y: { set: ['Country', 'Value 3 (+)'] }
            },
            title: 'Change Continuous',
            geometry: 'area',
            orientation: 'horizontal'
        } 
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: { set: 'Value 3 (+)' },
                color: { set: null }
            },
            title: 'Change Discrete'
        }
    }
    ),

    chart => chart.animate({
    config: {
        channels: {
            y: { set: ['Joy factors', 'Value 3 (+)'] },
            color: { set: 'Joy factors' }
        },
        title: 'Stacked Area Chart'
    } 
}

)];

export default testSteps;