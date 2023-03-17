import { data_4 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_4,


        config: {
            channels: {
                color: 'Country',
                size: ['Year', 'Value 2 (+)'],
                label: 'Year'
            },
            title:'Title',
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                size: null,
                label: null
            },
            title:'Title',
        },
        style: {
            plot: {
                marker: {
                    colorPalette: null
                }
            }
        }  
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;