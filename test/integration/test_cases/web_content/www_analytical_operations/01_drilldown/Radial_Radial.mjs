import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add 2 new Dimensions to the Y-axis and the Color channel
- remove the Measure from the Label channel`
;

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