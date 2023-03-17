import { data_3 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_3,

        config: {
            channels: {
                x: 'Year',
                y: 'Value 1 (+)',
                color: 'Country',
                size: null,
                label: null
            },
            title:'Title',
            geometry: 'line'
        } 
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: null,
                y: null,
                color: 'Country',
                size: ['Year', 'Value 1 (+)'],
                label:'Value 1 (+)'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;