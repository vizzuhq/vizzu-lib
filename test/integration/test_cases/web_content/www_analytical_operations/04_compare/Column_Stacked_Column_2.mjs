import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- move the Dimension from the Y-axis to the X-axis, behind the one there (group)`
;

const testSteps = [
    chart => chart.animate({
        data: data_4,

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
                x: ['Year', 'Country' ],
                y: 'Value 2 (+)'
            },
            title:'Title'
        }
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;