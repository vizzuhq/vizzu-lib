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
                color: { set: null },
                size: { set: 'Country' }
            },
            title: 'Stack Disc & Remove Color'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                size: { set: 'Joy factors' }
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
                y: { set: 'Value 1 (+)' },
                color: { set: null },
                size: { set: 'Joy factors' }
            },
            title: 'Change Conti'
        }
    }),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 1 (+)' },
            color: { set: 'Joy factors' },
            size: { set: null }
        },
        title: 'Group new Disc & Add new Disc Color'
    } 
})];

export default testSteps;