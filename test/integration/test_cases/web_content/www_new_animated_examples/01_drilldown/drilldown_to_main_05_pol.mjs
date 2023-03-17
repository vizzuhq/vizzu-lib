import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: ['Country','Value 2 (+)'],
                y: { range: { min: '-200%' } },
                color: 'Country',
                label: 'Value 2 (+)'
            },
            title: 'Title',
            coordSystem: 'polar'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: ['Year', 'Value 2 (+)'],
                y: { set: 'Country', range: { min: '-30%' } },
                label: null
            },
            title: 'Title'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: { set: ['Country','Value 2 (+)'], range: { min: '-30%' } }
            },
            title: 'Title'
        },
        style: {
            plot: {
                marker:{
                    rectangleSpacing: '0.1em'
                }
            }
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;