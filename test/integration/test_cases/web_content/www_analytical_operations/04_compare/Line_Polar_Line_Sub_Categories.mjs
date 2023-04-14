import { data_8 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- add the Dimension which is on the Color channel to the Y-axis too
- switch the Geometry from Line to Area
- set the Split parameter to true`
;

const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                x: 'Year',
                y: 'Value 2 (+)',
                color: 'Country'
            },
            title:'Title',
            geometry: 'line',
            coordSystem: 'polar'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '-20%' } }, 
            },
            title:'Title',
            geometry: 'area',
            split: true
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;