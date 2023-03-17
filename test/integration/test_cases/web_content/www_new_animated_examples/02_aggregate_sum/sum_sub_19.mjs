import { data_6 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_6,

        config: {
            channels: {
                x: 'Year',
                y: ['Value 2 (+)', 'Country'],
                color: 'Country'
            },
            title:'Title',
            geometry: 'area',
            align: 'stretch'
        }
    }
),

    chart => chart.animate({
        config: {
            title:'Title',
            align: 'min'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;