import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add the Measure to the Label channel too
- set the Split parameter to true`
;

const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country'
            },
            title:'Title'
        }
    }
),
    chart => chart.animate({
        config: {
            channels: {
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
    chart => chart.feature('tooltip',true)
];

export default testSteps;