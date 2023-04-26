import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const description = 
`- remove the Dimension from Y-axis
- switch the Geometry from Area to Line`
;

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
            geometry: 'area'
        }
    }
),
    
    chart => chart.animate({
        config: {
            channels: {
                y: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'line'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;