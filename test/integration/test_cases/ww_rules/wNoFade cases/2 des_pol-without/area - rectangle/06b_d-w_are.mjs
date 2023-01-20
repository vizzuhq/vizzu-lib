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
                    y: { set: ['Joy factors', 'Value 2 (+)'] },
                    color: null
                },
                title: 'Change Disc & Remove Color'
            }
        }
        ),
    
        chart => chart.animate({
            config: {
                channels: {
                    y: { set: ['Joy factors', 'Value 3 (+)'] },
                    color: null
                },
                title: 'Change Conti'
            }
        }
        ),
    
        chart => chart.animate({
            config: {
                channels: {
                    y: { set: ['Joy factors', 'Value 3 (+)'] },
                    color: { set: 'Joy factors' }
                },
                title: 'Add new Disc Color'
            }
        }
        ),

chart => chart.animate({
    config: {
        channels: {
            x: null,
            y: null,
            color: { set: 'Joy factors' },
            size: { set: ['Year', 'Value 3 (+)'] }
        },
        title: 'Change Geoms & CoordSys',
        geometry: 'rectangle'
    }
},
{
     geometry: {
         delay: 0.7,
         duration: 0.5
     },
     x: {
         delay: 1
     }
 }
)];

export default testSteps;