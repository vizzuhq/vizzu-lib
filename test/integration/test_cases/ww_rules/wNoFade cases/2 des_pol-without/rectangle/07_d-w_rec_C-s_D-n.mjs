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
        config: {
            channels: {
                x: { set: ['Year', 'Value 3 (+)'] },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: null }
            },
            title: '1 Tile Stretch & Remove Color',
            align: 'stretch'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Joy factors','Value 3 (+)'] },
                y: { set: 'Value 2 (+)', range: { min: '0%', max: '100%' }  }
            },
            title: '2 Tile Change Disc',
            geometry: 'rectangle',
            orientation: 'horizontal'
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
                color: { set: 'Joy factors' },
                lightness: { set: 'Value 2 (+)' },
                size: { set: 'Value 3 (+)' }
            },
            title: 'Change CoordSys'
        }
    },
    {
     easing: 'cubic-bezier(0.65,0,0.65,1)',
         x: {
             delay: 0.5,
             duration: 0.75
         }, 
         y: {
             delay: 0,
             duration: 0.75
         }
     }
    )


];

export default testSteps;