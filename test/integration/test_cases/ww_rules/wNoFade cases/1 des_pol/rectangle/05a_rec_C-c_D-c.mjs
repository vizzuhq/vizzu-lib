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
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Stretch',
            align: 'stretch'
        }
    }
    ),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Joy factors', 'Value 3 (+)'] },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Change Discrete & Orientation',
        orientation: 'vertical'
    } 
},
       {
                duration: 0
        }

),

chart => chart.animate({
    config: {
        channels: {
            y: { set: ['Country', 'Value 1 (+)'] }
        },
        title: 'Change Continuous'
    } 
}),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Joy factors', 'Value 3 (+)'] },
            y: { set: ['Country', 'Value 1 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Mekko Chart Stretch off & Change Orient.',
        orientation: 'horizontal',
        align: 'none'
    } 
}

)];

export default testSteps;