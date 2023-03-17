import { data_14 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_14,

        config: {
            channels: {
                noop: ['Year', 'Country'],
                size: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                noop: ['Year', 'Country'],
                y: 'Value 2 (+)',
                size: null
            },
            title:'Title'
        } 
    }
)
];

export default testSteps;