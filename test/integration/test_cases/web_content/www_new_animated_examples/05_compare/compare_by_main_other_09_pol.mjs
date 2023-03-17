import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Year',
                y: ['Value 1 (+)', 'Country'],
                color: 'Country'
            },
            title:'Title',
            geometry: 'area',
            split: true,
            coordSystem: 'polar'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                y: 'Value 1 (+)'
            },
            title:'Title',
            geometry: 'line',
            split: false
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;