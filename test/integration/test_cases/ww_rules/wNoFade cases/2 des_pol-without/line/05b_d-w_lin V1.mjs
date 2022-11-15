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
                size: { set: 'Value 3 (+)' }
            },
            title: 'Line Chart',
            geometry: 'line'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                size: { set: ['Country', 'Value 3 (+)'] }
            },
            title: 'Stack Disc & Remove Color'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: null  },
                size: { set: ['Country', 'Value 1 (+)'] }
            },
            title: 'Change Conti'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: null },
                size: { set: ['Joy factors', 'Value 1 (+)'] }
            },
            title: 'Change Disc'
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
                size: { set: ['Year', 'Value 3 (+)'] },
                lightness: { set: 'Value 1 (+)' }
            },
            title: 'Add new Disc Color & Change Geoms & CoordSys',
            geometry: 'circle'
        }
    },
    {
      x: {
          delay: 0.5
      }
     }
    )

];

export default testSteps;