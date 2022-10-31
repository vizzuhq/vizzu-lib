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
            orientation: 'horizontal'
        } 
    }),

    chart => chart.animate({
        data: {
            filter: record => 
            record['Joy factors'] != 'Happiness',
        },
        config: {
            channels: {
                x: { set: null },
                y: { set: null },
                color: { set: 'Joy factors' },
                lightness: { set: 'Value 2 (+)' },
                size: { set: 'Value 3 (+)' }
            },
            title: 'Filter'
        }
    }
    )


];

export default testSteps;