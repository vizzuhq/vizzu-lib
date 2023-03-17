import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Value 2 (+)',
                y: { set: 'Country', range: { min: '-30%' } }, 
                label: 'Value 2 (+)'
            },
            title:'Title',
            coordSystem: 'polar'
        }
    }
),
    chart => chart.animate({
        config: {
            channels: {
                x: ['Value 2 (+)', 'Joy factors'],
                color: 'Joy factors',
                label: null
            },
            title:'Title'
        },
        style: {
            plot: {
                marker: {
                    colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF'
                }
            }
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;