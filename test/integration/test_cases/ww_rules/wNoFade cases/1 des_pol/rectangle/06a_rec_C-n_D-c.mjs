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
            title: 'Stacked Column Chart'
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar',
            orientation: 'vertical'
        }
    }
    ),

    
    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: 'Country' },
            color: { set: 'Country' }
        },
        title: 'Bar'
    } 
},
       {
                duration: 0
        }

),
chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Joy factors' },
            y: { set: ['Country', 'Value 4 (+/-)'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Column Chart (negative sum)',
        orientation: 'horizontal'
    } 
}
)];

export default testSteps;