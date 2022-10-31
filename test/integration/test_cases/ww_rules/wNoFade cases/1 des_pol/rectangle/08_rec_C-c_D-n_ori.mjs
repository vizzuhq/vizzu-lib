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
                y: { set: ['Year', 'Country', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: 'Stack & Remove Color',
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
            title: 'Change Discrete'
        }
    },
    {
        duration: 0
    }
    ),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Value 1 (+)' },
                y: { set: ['Joy factors', 'Value 2 (+)'] },
                color: { set: 'Joy factors' }
            },
            title: 'Mekko Chart add Color',
            orientation: 'vertical'
        }
    }
)];

export default testSteps;