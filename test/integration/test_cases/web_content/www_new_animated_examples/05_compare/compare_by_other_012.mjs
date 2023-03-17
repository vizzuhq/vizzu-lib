import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: ['Country', 'Year'],
                y: 'Value 2 (+)',
                color: 'Country'
            },
            title:'Title'
        }
    }
),
    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
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