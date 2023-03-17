import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Year',
                y: ['Country', 'Value 2 (+)'],
                color: 'Country',
                size: null,
                label: null
            },
            title:'Title',
            geometry: 'area'
        } 
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: null,
                y: null ,
                color: 'Country',
                size: ['Year', 'Value 3 (+)'],
                label:'Year'
            },
            title:'Title',
            geometry: 'rectangle'
        }
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;