import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Country',
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
                x: 'Value 4 (+/-)',
                color: 'Country',
                label: 'Country'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;