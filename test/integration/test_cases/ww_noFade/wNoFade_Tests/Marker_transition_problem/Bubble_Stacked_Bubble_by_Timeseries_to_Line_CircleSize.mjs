import { data_3 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- Move the Size channel's Dimension to the X-axis
- Move the Measure to the Y-axis
- Null the Label channel
- switch the Geometry from Line to Circle`
;

const testSteps = [
    chart => chart.animate({
        data: data_3,
        config: {
            channels: {
                x: null,
                y: null,
                color: 'Country',
                size: ['Year', 'Value 1 (+)'],
//                label:'Value 1 (+)'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                x: 'Year',
                y: 'Value 1 (+)',
                color: 'Country',
                size: 'Value 1 (+)',
//                label: null
            },
            title:'Title',
            geometry: 'line'
        } 
    }
),
chart => chart.feature('tooltip',true)
];

export default testSteps;