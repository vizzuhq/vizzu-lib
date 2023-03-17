import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Country',
                y: ['Value 2 (+)', 'Joy factors'],
                color: 'Joy factors',
                label: 'Value 2 (+)'
            },
            title:'Title'
        }
    }
),

chart => chart.animate({
        config: {
            channels: {
                y: 'Value 2 (+)',
                color: null
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;