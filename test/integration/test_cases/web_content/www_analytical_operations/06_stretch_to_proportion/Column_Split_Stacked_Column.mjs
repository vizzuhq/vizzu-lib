import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- set the Align parameter to 'stretch'
- set the the Split parameter to false`
;

const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
                label: 'Value 2 (+)'
            },
            title:'Title',
            split: true
        },
        style: {
            plot: {
                marker:{
                    label: {
                        position:'top',
                        fontSize: '0.6em'
                    }
                }
            }
        }
    }
),

     chart => chart.animate({
        config: {
            channels: {
                label: null
            },
            title:'Title',
            align: 'stretch',
            split: false
        }
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;