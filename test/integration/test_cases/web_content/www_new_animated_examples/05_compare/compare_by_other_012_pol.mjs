import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: ['Country', 'Year'],
                y: { set: 'Value 2 (+)', range: { min: '-20%' } }, 
                color: 'Country'
            },
            title:'Title',
            coordSystem: 'polar'
        }
    }
),
    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)']
            },
            title:'Title',
            split: true
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;