import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`Add a new Dimension to the X-axis.`
;

const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
                label: 'Value 2 (+)'
            },
            title: 'Title'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                label: null
            },
            title: 'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;