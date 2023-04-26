import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- set Split parameter from true to false
- null Label channel
- + if you like set the marker-label fontSize in the Style`
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
            split: false
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;