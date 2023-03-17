import { data_6 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Value 2 (+)',
                y: 'Country',
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