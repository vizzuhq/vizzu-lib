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
            geometry: 'rectangle',
            orientation: 'horizontal'
        } 
    }),


    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: 'Country' },
                color: { set: 'Country' }
            },
            title: 'Bar, Remove Conti'
        }
    }
    ),

    
    chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Joy factors', 'Value 3 (+)'] },
            y: { set: ['Country'] },
            color: { set: 'Country' }
        },
        title: 'Bar, Change Disc'
    } 
},
       {
                duration: 0
        }

),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: null },
            color: { set: 'Country' },
            lightness: { set: 'Value 1 (+)' },
            noop: { set: 'Joy factors' },
            size: { set: ['Joy factors', 'Value 3 (+)'] }
        },
        title: 'Change Geoms & CoordSys'
    } 
},
{
 easing: 'cubic-bezier(0.65,0,0.65,1)',
     x: {
         delay: 0,
         duration: 0.75
     }, 
     y: {
         delay: 0.5,
         duration: 0.75
     }
 }

)];

export default testSteps;