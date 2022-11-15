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
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: null }
            },
            title: 'Remove Discrete & Color',
            geometry: 'area'
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 3 (+)' },
                color: { set: null }
            },
            title: 'Change Continuous'
        }
    }
    ),

    chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Joy factors', 'Value 3 (+)'] },
            color: { set: null },
            noop: { set: 'Year' },
            size: { set: 'Value 3 (+)' }
        },
        title: 'Add Discrete'
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
            noop: { set: 'Year' },
            size: { set: ['Year', 'Value 3 (+)'] }
        },
        title: 'Change Geoms & CoordSys',
        geometry: 'circle'
    } 
},
{
     geometry: { 
         delay: 0, 
         duration: 0.5
     }
 }

),

chart => chart.animate({
    data: {
        filter: record => 
        record['Joy factors'] != 'Creativity' &&
        record['Joy factors'] != 'Love',
    },
    config: {
        channels: {
            x: { set: null },
            y: { set: null },
            color: { set: 'Joy factors' },
            noop: { set: 'Year' },
            size: { set: ['Year', 'Value 3 (+)'] }
        },
        title: 'Change Geoms & CoordSys',
        geometry: 'circle'
    } 
}
)

];

export default testSteps;