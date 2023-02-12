import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                ['AT', 'DE', 'FI', 'ES']
                .includes(record.Country_code)
        }),
        config: {
            channels: {
                x: 'Year',
                y: 'Value 3 (+)'
                
                
            },
            title: 'Single Line Chart',
            geometry: 'line'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: ['Country', 'Value 3 (+)'],
                color: 'Country'
            },
            title: 'Drill down',
            geometry: 'area'
        }
    }),
    
    chart => chart.animate({
        config: {
            channels: {
                y: 'Value 3 (+)'
            },
            title: 'Line Chart I',
            geometry: 'line'
        }
    })
];

export default testSteps;