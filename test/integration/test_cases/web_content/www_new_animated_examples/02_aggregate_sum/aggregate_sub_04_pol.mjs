import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Country',
                y: ['Value 2 (+)', 'Joy factors'],
                color: 'Joy factors'
            },
            title:'Title',
            coordSystem: 'polar'
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

chart => chart.animate({
        config: {
            channels: {
                y: 'Value 2 (+)',
                color: null,
                label: 'Value 2 (+)'
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;