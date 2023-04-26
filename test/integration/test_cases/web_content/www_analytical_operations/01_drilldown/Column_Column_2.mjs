import { data_4 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add a new Dimension to the X-axis
- and another to the Y axis and the Color channel`
;

const testSteps = [
    chart => chart.animate({
        data: data_4,

        config: {
            channels: {
                y: 'Value 2 (+)',
                label: 'Value 2 (+)'
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
                color: 'Country',
                label: null
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;